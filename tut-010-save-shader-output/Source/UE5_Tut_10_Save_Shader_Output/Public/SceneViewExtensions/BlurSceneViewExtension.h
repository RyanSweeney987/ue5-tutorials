#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

struct FBlurRequestData;
class UTexture;

class UE5_TUT_10_SAVE_SHADER_OUTPUT_API FBlurSceneViewExtension final : public FSceneViewExtensionBase
{
	FSceneViewExtensionIsActiveFunctor IsActiveFunctor;
		
	TUniquePtr<FRHIGPUTextureReadback> Readback;
		
	bool bHasQueuedTexture = true;
	bool bImmediateFetch = false;
	bool bHasPendingReadback = false;
	
	FIntPoint ReadbackTextureExtent = FIntPoint::ZeroValue;
	
	float BlurRadius = 1.0f;
	
	TWeakObjectPtr<UTexture> SourceTexture;
	FTextureRHIRef SourceRHITexture = nullptr;
	
	DECLARE_DELEGATE_TwoParams(FOnBlurRequestCompleted, const TArray64<float>&, const FIntPoint&);
	FOnBlurRequestCompleted OnBlurRequestCompletedDelegate;
public:
	FBlurSceneViewExtension(const FAutoRegister& AutoRegister);
	
	void QueueBlurRequest_GameThread(const FBlurRequestData& BlurRequestData);
	
	//-----------------------------------------------------------------------------------
	// Scene View Extension Implementation
	//-----------------------------------------------------------------------------------
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	// virtual void SetupViewPoint(APlayerController* Player, FMinimalViewInfo& InViewInfo) {}
	// virtual void SetupViewProjectionMatrix(FSceneViewProjectionData& InOutProjectionData) {}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}
	// virtual void PostCreateSceneRenderer(const FSceneViewFamily& InViewFamily, ISceneRenderer* Renderer) {}
	// virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) {}
	virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override {}
	// virtual void PreInitViews_RenderThread(FRDGBuilder& GraphBuilder) {}
	// virtual void PreRenderBasePass_RenderThread(FRDGBuilder& GraphBuilder, bool bDepthBufferIsPopulated) {}
	// virtual void PostRenderBasePassDeferred_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView, const FRenderTargetBindingSlots& RenderTargets, TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTextures) {}
	// virtual void PostRenderBasePassMobile_RenderThread(FRHICommandList& RHICmdList, FSceneView& InView) {}
	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& InView, const FPostProcessingInputs& Inputs) override;

	// virtual void SubscribeToPostProcessingPass(EPostProcessingPass Pass, const FSceneView& InView, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled) override;

	//-----------------------------------------------------------------------------
	
	FOnBlurRequestCompleted& OnBlurRequestCompleted() { return OnBlurRequestCompletedDelegate; }
private:
	void ProcessReadback();
};
