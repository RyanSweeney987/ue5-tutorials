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

FBlurSceneViewExtension::FBlurSceneViewExtension(const FAutoRegister& AutoRegister)
	: FSceneViewExtensionBase(AutoRegister)
{
}

void FBlurSceneViewExtension::SetSourceTexture(UTexture* InTexture)
{
	FScopeLock Lock(&SourceTextureMutex);
	SourceTexture = InTexture;
}

void FBlurSceneViewExtension::QueueBlurRequest_RenderThread(FReadbackCallback InCallback, const bool bInDownloadImmediately)
{
	check(IsInRenderingThread());

	FScopeLock Lock(&SourceTextureMutex);
	PendingReadbackCallback = MoveTemp(InCallback);
	bPendingDownloadImmediately = bInDownloadImmediately;
	bHasPendingRequest = PendingReadbackCallback != nullptr;
}

void FBlurSceneViewExtension::PrePostProcessPass_RenderThread(
	FRDGBuilder& GraphBuilder,
	const FSceneView& InView,
	const FPostProcessingInputs& Inputs)
{
	TWeakObjectPtr<UTexture> LocalSourceTexture;
	FReadbackCallback LocalCallback;
	bool bDownloadImmediately = false;
	{
		FScopeLock Lock(&SourceTextureMutex);
		LocalSourceTexture = SourceTexture;
		if (!bHasPendingRequest)
		{
			return;
		}

		LocalCallback = MoveTemp(PendingReadbackCallback);
		PendingReadbackCallback = nullptr;
		bDownloadImmediately = bPendingDownloadImmediately;
		bPendingDownloadImmediately = false;
		bHasPendingRequest = false;
	}

	UTexture* SourceTextureObject = LocalSourceTexture.Get();
	if (!SourceTextureObject || !SourceTextureObject->GetResource() || !SourceTextureObject->GetResource()->TextureRHI.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("BlurSceneViewExtension: source texture is invalid."));
		return;
	}

	const FTextureRHIRef SourceTextureRHI = SourceTextureObject->GetResource()->TextureRHI;
	const int32 Width = SourceTextureObject->GetSurfaceWidth();
	const int32 Height = SourceTextureObject->GetSurfaceHeight();

	FRDGTextureRef InputTextureRDG = GraphBuilder.RegisterExternalTexture(
		CreateRenderTarget(SourceTextureRHI, TEXT("BlurInputTexture")));

	const FRDGTextureDesc OutputDesc = FRDGTextureDesc::Create2D(
		FIntPoint(Width, Height),
		PF_B8G8R8A8,
		FClearValueBinding::Black,
		TexCreate_ShaderResource | TexCreate_RenderTargetable);

	FRDGTextureRef OutputTexture = GraphBuilder.CreateTexture(OutputDesc, TEXT("BlurOutputTexture"));

	FBlurPS::FParameters* Parameters = GraphBuilder.AllocParameters<FBlurPS::FParameters>();
	Parameters->InputTexture = InputTextureRDG;
	Parameters->InputSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
	Parameters->TexelSize = FVector2f(1.0f / FMath::Max(1, Width), 1.0f / FMath::Max(1, Height));
	Parameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::ENoAction);

	TShaderMapRef<FBlurPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	FPixelShaderUtils::AddFullscreenPass(
		GraphBuilder,
		GetGlobalShaderMap(GMaxRHIFeatureLevel),
		RDG_EVENT_NAME("BlurPS"),
		PixelShader,
		Parameters,
		FIntRect(0, 0, Width, Height));

	const TSharedRef<FRHIGPUTextureReadback, ESPMode::ThreadSafe> Readback =
		MakeShared<FRHIGPUTextureReadback, ESPMode::ThreadSafe>(TEXT("BlurOutputReadback"));
	AddEnqueueCopyPass(GraphBuilder, &Readback.Get(), OutputTexture, FResolveRect());

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask,
		[Readback, Width, Height, Callback = MoveTemp(LocalCallback), bDownloadImmediately]() mutable
		{
			if (!bDownloadImmediately)
			{
				while (!Readback->IsReady())
				{
					FPlatformProcess::Sleep(0.005f);
				}
			}

			TArray64<uint8> PixelData;
			PixelData.SetNumZeroed(static_cast<int64>(Width) * static_cast<int64>(Height) * 4);

			int32 RowPitchInPixels = 0;
			if (void* ReadbackData = Readback->Lock(RowPitchInPixels, nullptr))
			{
				const uint8* SourcePtr = static_cast<const uint8*>(ReadbackData);
				const int64 DestRowBytes = static_cast<int64>(Width) * 4;
				const int64 SourceRowBytes = static_cast<int64>(RowPitchInPixels) * 4;

				for (int32 Row = 0; Row < Height; ++Row)
				{
					FMemory::Memcpy(
						PixelData.GetData() + (static_cast<int64>(Row) * DestRowBytes),
						SourcePtr + (static_cast<int64>(Row) * SourceRowBytes),
						DestRowBytes);
				}

				Readback->Unlock();
			}

			AsyncTask(ENamedThreads::GameThread,
				[Callback = MoveTemp(Callback), PixelData = MoveTemp(PixelData), Width, Height]() mutable
				{
					if (Callback)
					{
						Callback(MoveTemp(PixelData), Width, Height);
					}
				});
		});
}

void FBlurSceneViewExtension::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
}

void FBlurSceneViewExtension::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
}

void FBlurSceneViewExtension::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
}
