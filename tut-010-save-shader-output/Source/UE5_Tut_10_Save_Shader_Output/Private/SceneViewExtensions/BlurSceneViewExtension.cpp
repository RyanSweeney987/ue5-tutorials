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
#include "ShaderPasses/BlurPS.h"

// Useful link https://docs.clusterfact.games/docs/Snippets/

FBlurSceneViewExtension::FBlurSceneViewExtension(const FAutoRegister& AutoRegister, const TFunction<void(TArray64<uint8>&, FIntPoint&)>& InCallbackFunction)
	: FSceneViewExtensionBase(AutoRegister),
	CallbackFunction(InCallbackFunction)
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

void FBlurSceneViewExtension::QueueBlurRequest_GameThread(UTexture* InTexture, const bool bInDownloadImmediately)
{
	checkf(IsInGameThread(), TEXT("QueueBlurRequest_GameThread must be called from the game thread."));
	
	bHasQueuedTexture = true;
	SourceTexture = InTexture;
	ReadbackTextureExtent = FIntPoint(SourceTexture->GetSurfaceWidth(), SourceTexture->GetSurfaceHeight());
	bImmediateFetch = bInDownloadImmediately;
}

void FBlurSceneViewExtension::SetCallbackFunction(const TFunction<void(TArray64<uint8>&, FIntPoint&)>& InCallbackFunction)
{
	CallbackFunction = InCallbackFunction;
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

	// Create the output texture, it should be the same as the input texture
	FRDGTextureDesc OutputTextureDesc = InputTexture->Desc;
	OutputTextureDesc.Flags |= TexCreate_RenderTargetable | TexCreate_ShaderResource;
	const FRDGTextureRef OutputTexture = GraphBuilder.CreateTexture(OutputTextureDesc, TEXT("BlurOutputTexture"));

	// Setup parameters
	FBlurPS::FParameters* Parameters = GraphBuilder.AllocParameters<FBlurPS::FParameters>();
	Parameters->InputTexture = InputTexture;
	Parameters->InputSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
	Parameters->TexelSize = FVector2f(1.0f / TextureExtent.X, 1.0f / TextureExtent.Y);
	Parameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::ENoAction);
	
	// Run the shader
	const TShaderMapRef<FBlurPS> PixelShader(GlobalShaderMap);
	FPixelShaderUtils::AddFullscreenPass(
		GraphBuilder,
		GlobalShaderMap,
		RDG_EVENT_NAME("BlurPS"),
		PixelShader,
		Parameters,
		Viewport);
	
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
		// at the beginning of the function
		ProcessReadback();
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

void FBlurSceneViewExtension::ProcessReadback()
{
	// Pre UE 5.5 - lower level
	// TRefCountPtr<FRHIStagingBuffer> Staging = RHICreateStagingBuffer();
	// RHICmdList.CopyToStagingBuffer(SrcRHI, Staging, Offset, Size);
	
	// NOTE: The data coming out needs to match what you're expecting to put in the saved texture
	
	// Setup the temporary array that will pass on the data
	TArray64<uint8> PixelData;
	PixelData.SetNumZeroed(static_cast<int64>(ReadbackTextureExtent.X) * static_cast<int64>(ReadbackTextureExtent.Y) * 4);
	
	// Read the data
	int32 RowPitchInPixels = 0;
	if (const void* ReadbackData = Readback->Lock(RowPitchInPixels, nullptr))
	{
		// Get the starting position
		const uint8* SourcePtr = static_cast<const uint8*>(ReadbackData);
		const int64 DestRowBytes = static_cast<int64>(ReadbackTextureExtent.X) * 4;
		const int64 SourceRowBytes = static_cast<int64>(RowPitchInPixels) * 4;
	
		for (int32 Row = 0; Row < ReadbackTextureExtent.Y; ++Row)
		{
			// Copy the data
			FMemory::Memcpy(PixelData.GetData() + (static_cast<int64>(Row) * DestRowBytes),
				SourcePtr + (static_cast<int64>(Row) * SourceRowBytes),
				DestRowBytes);
		}
	
		Readback->Unlock();
		// Clear the readback pointer now that we're done with it
		Readback.Reset();
	}
	
	TFunction<void(TArray64<uint8>&, FIntPoint&)> LocalCallback = CallbackFunction;
	FIntPoint LocalExtent = ReadbackTextureExtent;
	// When it's done, return the results to the callback function
	AsyncTask(ENamedThreads::GameThread, [PixelData = MoveTemp(PixelData), LocalExtent, Callback = MoveTemp(LocalCallback)]() mutable
	{
		if(Callback)
		{
			Callback(PixelData, LocalExtent);
		}
	});
}


