#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

class UTexture;

class UE5_TUT_10_STATIC_TEXTURE_IN_SHADER_API FBlurSceneViewExtension final : public FSceneViewExtensionBase
{
public:
	using FReadbackCallback = TFunction<void(TArray64<uint8>&&, int32, int32)>;

	explicit FBlurSceneViewExtension(const FAutoRegister& AutoRegister);

	void SetSourceTexture(UTexture* InTexture);
	void QueueBlurRequest_RenderThread(FReadbackCallback InCallback, bool bInDownloadImmediately);

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void PrePostProcessPass_RenderThread(
		FRDGBuilder& GraphBuilder,
		const FSceneView& InView,
		const FPostProcessingInputs& Inputs) override;

private:
	FCriticalSection SourceTextureMutex;
	TWeakObjectPtr<UTexture> SourceTexture;
	FReadbackCallback PendingReadbackCallback;
	bool bPendingDownloadImmediately = false;
	bool bHasPendingRequest = false;
};
