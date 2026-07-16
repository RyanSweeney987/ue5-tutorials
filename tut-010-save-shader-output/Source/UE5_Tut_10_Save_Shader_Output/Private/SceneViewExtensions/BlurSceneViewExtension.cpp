#include "SceneViewExtensions/BlurSceneViewExtension.h"

#include "Async/Async.h"
#include "Engine/Texture.h"
#include "HAL/PlatformProcess.h"
#include "PixelShaderUtils.h"
#include "RHICommandList.h"
#include "RHIStaticStates.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "RendererInterface.h"
#include "Data/BlurRequests.h"
#include "ShaderPasses/BlurPS.h"

// Useful link https://docs.clusterfact.games/docs/Snippets/

void GetPassCounts(const float BlurRadius, int32& Kernel3X3Passes, int32& Kernel5X5Passes)
{
	// Convert requested blur radius to Gaussian sigma (radius ~= 3*sigma)
	const float TargetSigma = FMath::Max(0.0f, BlurRadius) / 3.0f;
	const float TargetVariance = TargetSigma * TargetSigma;

	// From your binomial kernels:
	// 3x3 => sigma^2 ~= 0.5 per pass
	// 5x5 => sigma^2 ~= 1.0 per pass
	constexpr float VariancePer5X5Pass = 1.0f;
	constexpr float VariancePer3X3Pass = 0.5f;

	// Quantize to the nearest representable variance step (0.5) so 3x3 fills only the remainder
	const float QuantizedVariance = FMath::RoundToFloat(TargetVariance / VariancePer3X3Pass) * VariancePer3X3Pass;

	// Prefer 5x5 passes first, then 3x3 for the remaining half-step
	Kernel5X5Passes = FMath::FloorToInt(QuantizedVariance / VariancePer5X5Pass);
	const float RemainingVariance = QuantizedVariance - (Kernel5X5Passes * VariancePer5X5Pass);
	Kernel3X3Passes = FMath::RoundToInt(RemainingVariance / VariancePer3X3Pass); // 0 or 1

	// Optional: ensure non-zero work when BlurRadius > 0
	if (BlurRadius > 0.0f && Kernel5X5Passes == 0 && Kernel3X3Passes == 0)
	{
		Kernel3X3Passes = 1;
	}
}

FBlurSceneViewExtension::FBlurSceneViewExtension(const FAutoRegister& AutoRegister)
	: FSceneViewExtensionBase(AutoRegister)
{
	// Only run if we have a texture applied to the scene view extension
	IsActiveFunctor.IsActiveFunction = TSceneViewExtensionIsActiveFunction(
		[this](const ISceneViewExtension* SceneViewExtension, const FSceneViewExtensionContext& Context)
		{
			// Run if we have either a texture queued or a readback pending
			return TOptional<bool>(bHasQueuedTexture || bHasPendingReadback);
		}
	);
	
	IsActiveThisFrameFunctions.Add(IsActiveFunctor);
}

void FBlurSceneViewExtension::QueueBlurRequest_GameThread(const FBlurRequestData& BlurRequestData)
{
	checkf(IsInGameThread(), TEXT("QueueBlurRequest_GameThread must be called from the game thread."));
	
	bHasQueuedTexture = true;
	BlurRadius = BlurRequestData.BlurRadius;
	SourceTexture = BlurRequestData.Texture;
	ReadbackTextureExtent = FIntPoint(SourceTexture->GetSurfaceWidth(), SourceTexture->GetSurfaceHeight());
	bImmediateFetch = BlurRequestData.bInDownloadImmediately;
}

void FBlurSceneViewExtension::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
	if(SourceTexture.IsValid())
	{
		if(const FTextureResource* Resource = SourceTexture->GetResource())
		{
			// Set the texture parameter for the scene view
			SourceRHITexture = Resource->TextureRHI;
		}
		
		// We have effectively consumed it so reset it
		SourceTexture.Reset();
	} else
	{
		// Make sure this is cleared as it should have already been used once
		SourceRHITexture = nullptr;
	}
}

void FBlurSceneViewExtension::PrePostProcessPass_RenderThread(
	FRDGBuilder& GraphBuilder,
	const FSceneView& InView,
	const FPostProcessingInputs& Inputs)
{
	// Check for any readback first
	if(bHasPendingReadback)
	{
		// If the pointer isn't set, or if the readback extent is zero, early exit
		if (!Readback.IsValid() || ReadbackTextureExtent.X <= 0 || ReadbackTextureExtent.Y <= 0)
		{
			Readback.Reset();
			ReadbackTextureExtent = FIntPoint::ZeroValue;
			bHasPendingReadback = false;
			
			// UE_LOG(LogTemp, Warning, TEXT("BlurSceneViewExtension: Readback not valid or extent is zero, readback cannot be performed."));
			
			return;
		}

		// If it's not ready, try again next pass
		if (!Readback->IsReady())
		{
			return; 
		}
		
		// Process the readback
		ProcessReadback();
		
		ReadbackTextureExtent = FIntPoint::ZeroValue;
		bHasPendingReadback = false;
		
		// We only wanted to do a readback here
		return;
	}
	
	// Check if we have a source texture
	if(!SourceRHITexture.IsValid())
	{
		// Commented out to prevent log polution
		// UE_LOG(LogTemp, Warning, TEXT("BlurSceneViewExtension: Source RHI texture not set"));
		return;
	}
	
	// The size of the texture will be used as the render viewport size
	const FIntPoint TextureExtent = FIntPoint(SourceRHITexture->GetSizeX(), SourceRHITexture->GetSizeY());
	const FIntRect Viewport = FIntRect(0, 0, TextureExtent.X, TextureExtent.Y);
	
	const FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);

	// Create the source texture ref
	const FRDGTextureRef InputTexture = GraphBuilder.RegisterExternalTexture(CreateRenderTarget(SourceRHITexture, TEXT("BlurInputTexture")));
	
	// Calculate the number of passes required
	int32 Kernel3X3Passes = 0, Kernel5X5Passes = 0;
	GetPassCounts(BlurRadius, Kernel3X3Passes, Kernel5X5Passes);
	
	int32 TotalPasses = Kernel3X3Passes + Kernel5X5Passes;
	
	FRDGTextureRef PreviousInput = InputTexture;
	FRDGTextureRef OutputTexture = nullptr;
	
	while(TotalPasses > 0)
	{
		// Create the output texture, it should be the same as the input texture
		FRDGTextureDesc OutputTextureDesc = InputTexture->Desc;
		OutputTextureDesc.Format = PF_A32B32G32R32F; // Use a float format to avoid precision loss
		OutputTextureDesc.Flags |= TexCreate_RenderTargetable | TexCreate_ShaderResource;
		OutputTexture = GraphBuilder.CreateTexture(OutputTextureDesc, TEXT("OutputTexture"));
		
		// Setup parameters
		FBlurPS::FParameters* Parameters = GraphBuilder.AllocParameters<FBlurPS::FParameters>();
		Parameters->InputTexture = PreviousInput;
		Parameters->InputSampler = TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
		Parameters->TexelSize = FVector2f(1.0f / TextureExtent.X, 1.0f / TextureExtent.Y);
		Parameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::ENoAction);
		
		// Setup the permutation vector so it sets whether to use the larger or smaller kernel
		FBlurPS::FPermutationDomain PermutationVector;
		PermutationVector.Set<FBlurPS::FLargeKernel>(Kernel5X5Passes > 0);
		
		const TShaderMapRef<FBlurPS> PixelShader(GlobalShaderMap, PermutationVector);
		
		// Run the shader
		FPixelShaderUtils::AddFullscreenPass(
			GraphBuilder,
			GlobalShaderMap,
			RDG_EVENT_NAME("BlurPS"),
			PixelShader,
			Parameters,
			Viewport);
		
		// Set the previous input to be the current output
		PreviousInput = OutputTexture;
		
		// Remove from the 5x5 kernel first, then from kernel 3x3
		if(Kernel5X5Passes > 0)
		{
			Kernel5X5Passes--;
		} else
		{
			Kernel3X3Passes--;
		}
		// Update the total pass count
		TotalPasses = Kernel3X3Passes + Kernel5X5Passes;
	};
	
	// Create the readback buffer if it doesn't exist, this will get reset when the copy has completed
	if(!Readback.IsValid())
	{
		Readback = MakeUnique<FRHIGPUTextureReadback>(TEXT("BlurOutputReadback"));
	}
	
	// Enqueue a readback
	AddEnqueueCopyPass(GraphBuilder, Readback.Get(), OutputTexture);
	
	if(bImmediateFetch)
	{
		// If immediate fetch is enabled, process it straight away, otherwise we check if it's ready 
		// at the beginning of the function which runs every frame when enabled
		GraphBuilder.AddPass(
		   RDG_EVENT_NAME("BlurReadbackSync"),
		   ERDGPassFlags::None,
		   [this](FRHICommandListImmediate& RHICmdList)
		   {
			   // Force all queued GPU work (including the copy) to complete now
			   RHICmdList.ImmediateFlush(EImmediateFlushType::FlushRHIThreadFlushResources);
			   RHICmdList.BlockUntilGPUIdle();

			   ProcessReadback();
			   ReadbackTextureExtent = FIntPoint::ZeroValue;
			   bHasPendingReadback = false;
		   });
	} else
	{
		// Set this to true so we can still run the scene view extension
		bHasPendingReadback = true;
	}
	
	// We no longer have a queued texture as we have used it
	bHasQueuedTexture = false;
	// Reset this as we have effectively consumed it
	SourceRHITexture = nullptr;
}

/**
 * NOTE: The data coming out needs to match what you're expecting to put in the saved texture
 * NOTE: This code works with textures that are set to the RGBA8 format or Compression Settings set to UserInterface2D
 */
void FBlurSceneViewExtension::ProcessReadback()
{
	// Pre UE 5.5 - lower level
	// TRefCountPtr<FRHIStagingBuffer> Staging = RHICreateStagingBuffer();
	// RHICmdList.CopyToStagingBuffer(SrcRHI, Staging, Offset, Size);
		
	// Setup the temporary array that will pass on the data
	TArray64<float> PixelData;
	PixelData.SetNumZeroed(static_cast<int64>(ReadbackTextureExtent.X) * static_cast<int64>(ReadbackTextureExtent.Y) * 4);
	
	// Read the data
	int32 RowPitchInPixels = 0;
	if (const void* ReadbackData = Readback->Lock(RowPitchInPixels, nullptr))
	{
		// Get the starting position
		const float* SourcePtr = static_cast<const float*>(ReadbackData);
		const int64 DestRowBytes = static_cast<int64>(ReadbackTextureExtent.X) * 4;
		const int64 SourceRowBytes = static_cast<int64>(RowPitchInPixels) * 4;
	
		for (int32 Row = 0; Row < ReadbackTextureExtent.Y; ++Row)
		{
			// Copy the data
			// Make sure to take into account the size of each color element
			FMemory::Memcpy(PixelData.GetData() + (static_cast<int64>(Row) * DestRowBytes),
				SourcePtr + (static_cast<int64>(Row) * SourceRowBytes),
				DestRowBytes * sizeof(float));
		}
	
		Readback->Unlock();
		// Clear the readback pointer now that we're done with it
		Readback.Reset();
	}
	
	FIntPoint LocalExtent = ReadbackTextureExtent;
	// When it's done, return the results to the callback function
	AsyncTask(ENamedThreads::GameThread, [this, PixelData = MoveTemp(PixelData), LocalExtent]() mutable
	{
		OnBlurRequestCompletedDelegate.ExecuteIfBound(PixelData, LocalExtent);
	});
}


