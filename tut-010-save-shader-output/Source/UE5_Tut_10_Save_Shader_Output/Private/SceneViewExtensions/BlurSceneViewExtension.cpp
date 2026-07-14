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
#include "Subsystems/SaveShaderOutputSubsystem.h"

FBlurSceneViewExtension::FBlurSceneViewExtension(const FAutoRegister& AutoRegister)
	: FSceneViewExtensionBase(AutoRegister)
{
	// Only run if we have a texture applied to the scene view extension
	IsActiveFunctor.IsActiveFunction = TSceneViewExtensionIsActiveFunction(
		[this](const ISceneViewExtension* SceneViewExtension, const FSceneViewExtensionContext& Context)
		{
			const USaveShaderOutputSubsystem* SaveShaderOutputSubsystem = USaveShaderOutputSubsystem::Get();
			if(!SaveShaderOutputSubsystem)
			{
				return TOptional<bool>(false);
			}
			
			// Only run if we have a source texture set
			return TOptional<bool>(SaveShaderOutputSubsystem->IsSourceTextureSet());
		}
	);
	
	IsActiveThisFrameFunctions.Add(IsActiveFunctor);
}

void FBlurSceneViewExtension::PrePostProcessPass_RenderThread(
	FRDGBuilder& GraphBuilder,
	const FSceneView& InView,
	const FPostProcessingInputs& Inputs)
{
	USaveShaderOutputSubsystem* SaveShaderOutputSubsystem = USaveShaderOutputSubsystem::Get();
	if (!SaveShaderOutputSubsystem)
	{
		// Can't do anything so return
		return;
	}

	// Consume the source texture for the pass, this would then stop the pass from running
	UTexture2D* SourceTexture = SaveShaderOutputSubsystem->ConsumeSourceTexture_RenderThread();
	if (!SourceTexture || !SourceTexture->GetResource() || !SourceTexture->GetResource()->TextureRHI.IsValid())
	{
		// If it's not a valid texture, return
		UE_LOG(LogTemp, Warning, TEXT("BlurSceneViewExtension: source texture is invalid."));
		return;
	}
	// Get the underlying RHI for the texture so we can actually use it in the shader
	const FTextureRHIRef SourceTextureRHI = SourceTexture->GetResource()->TextureRHI;
	// The size of the texture will be used as the render viewport size
	const FIntRect Viewport = FIntRect(0, 0, SourceTexture->GetSurfaceWidth(), SourceTexture->GetSurfaceHeight());
	
	const FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);

	// Create the source texture ref
	const FRDGTextureRef InputTexture = GraphBuilder.RegisterExternalTexture(CreateRenderTarget(SourceTextureRHI, TEXT("BlurInputTexture")));

	// Create the output texture, it should be the same as the input texture
	const FRDGTextureRef OutputTexture = GraphBuilder.CreateTexture(InputTexture->Desc, TEXT("BlurOutputTexture"));

	// Setup parameters
	FBlurPS::FParameters* Parameters = GraphBuilder.AllocParameters<FBlurPS::FParameters>();
	Parameters->InputTexture = InputTexture;
	Parameters->InputSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
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

	// Enqueue a readback
	
	// We have 2 paths here - async readback which is the more performant way to do it
	// However it could be a few frames until you get your results back
	// Or we do it immediately, this will lock up the CPU until the GPU catches up
	
	// const TSharedRef<FRHIGPUTextureReadback, ESPMode::ThreadSafe> Readback = 
	// 	MakeShared<FRHIGPUTextureReadback, ESPMode::ThreadSafe>(TEXT("BlurOutputReadback"));
	// AddEnqueueCopyPass(GraphBuilder, &Readback.Get(), OutputTexture, FResolveRect());
	//
	// // Readback or setting texture data found somewhere to do with PNG - ImageUtils
	//
	// AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask,
	// 	[Readback, Width, Height, Callback = MoveTemp(LocalCallback), bDownloadImmediately]() mutable
	// {
	// 	TArray64<uint8> PixelData;
	// 	PixelData.SetNumZeroed(static_cast<int64>(Width) * static_cast<int64>(Height) * 4);
	//
	// 	int32 RowPitchInPixels = 0;
	// 		// Calling this after execute would force the CPU to wait for the GPU to catch up
	// 	if (void* ReadbackData = Readback->Lock(RowPitchInPixels, nullptr))
	// 	{
	// 		const uint8* SourcePtr = static_cast<const uint8*>(ReadbackData);
	// 		const int64 DestRowBytes = static_cast<int64>(Width) * 4;
	// 		const int64 SourceRowBytes = static_cast<int64>(RowPitchInPixels) * 4;
	//
	// 		for (int32 Row = 0; Row < Height; ++Row)
	// 		{
	// 			FMemory::Memcpy(
	// 				PixelData.GetData() + (static_cast<int64>(Row) * DestRowBytes),
	// 				SourcePtr + (static_cast<int64>(Row) * SourceRowBytes),
	// 				DestRowBytes);
	// 		}
	//
	// 		Readback->Unlock();
	// 	}
	//
	// 	AsyncTask(ENamedThreads::GameThread,
	// 		[Callback = MoveTemp(Callback), PixelData = MoveTemp(PixelData), Width, Height]() mutable
	// 		{
	// 			if (Callback)
	// 			{
	// 				Callback(MoveTemp(PixelData), Width, Height);
	// 			}
	// 		});
	// });
}
