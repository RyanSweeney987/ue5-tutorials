#include "Rendering/CustomSceneViewExtension.h"

#include "PixelShaderUtils.h"
#include "RenderGraphEvent.h"
#include "SceneRenderTargetParameters.h"
#include "ScreenPass.h"
#include "SceneRendering.h"
#include "SceneTexturesConfig.h"
#include "Runtime/Renderer/Internal/PostProcess/PostProcessInputs.h"
#include "ShaderPasses/ColourExtractRenderPass.h"

DECLARE_GPU_DRAWCALL_STAT(ColourExtract);

FCustomSceneViewExtension::FCustomSceneViewExtension(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
{
}

void FCustomSceneViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	FSceneViewExtensionBase::PrePostProcessPass_RenderThread(GraphBuilder, View, Inputs);

	checkSlow(View.bIsViewInfo);
	const FIntRect Viewport = static_cast<const FViewInfo&>(View).ViewRect;
	const FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);

	RDG_GPU_STAT_SCOPE(GraphBuilder, ColourExtract);
	RDG_EVENT_SCOPE(GraphBuilder, "Colour Extract");

	const FSceneTextureShaderParameters SceneTextures = CreateSceneTextureShaderParameters(GraphBuilder, View, ESceneTextureSetupMode::SceneColor | ESceneTextureSetupMode::GBuffers);
	const FScreenPassTexture SceneColourTexture((*Inputs.SceneTextures)->SceneColorTexture, Viewport);

	FColourExtractPS::FParameters* Parameters = GraphBuilder.AllocParameters<FColourExtractPS::FParameters>();
	Parameters->SceneColorTexture = SceneColourTexture.Texture;
	Parameters->SceneTextures = SceneTextures;
	Parameters->TargetColour = FVector3f(1.0f, 0.0f, 0.0f);
	Parameters->View = View.ViewUniformBuffer;
	Parameters->RenderTargets[0] = FRenderTargetBinding((*Inputs.SceneTextures)->SceneColorTexture, ERenderTargetLoadAction::ELoad);

	// Set up our permutations
	FColourExtractPS::FPermutationDomain PermutationVector;
	// Simple true or false
	PermutationVector.Set<FColourExtractPS::FBoolPermutation>(true);
	// Starts at 0, count is 5 (0, 1, 2, 3, 4)
	PermutationVector.Set<FColourExtractPS::FIntPermutation>(3);
	// Starts at 2, count is 3 (2, 3, 4)
	PermutationVector.Set<FColourExtractPS::FIntRangePermutation>(4); 
	// Needs to be one of the specified values (16, 32, 64, 128, 256, 512)
	PermutationVector.Set<FColourExtractPS::FSparseIntPermutation>(32);
	// Would need to be a value of an enum, enums should have contiguous values starting at 0, so if you have 3 values, they would be 0, 1, 2
	// With EMyEnum::Value2 being 1
	PermutationVector.Set<FColourExtractPS::FEnumPermutation>(EMyEnum::Value2);
	
	// The permutation vector gets included here
	TShaderMapRef<FColourExtractPS> PixelShader(GlobalShaderMap, PermutationVector);
	FPixelShaderUtils::AddFullscreenPass(GraphBuilder, GlobalShaderMap, FRDGEventName(TEXT("Colour Extract Pass")), PixelShader, Parameters, Viewport);
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
void FCustomSceneViewExtension::SubscribeToPostProcessingPass(EPostProcessingPass Pass, const FSceneView& View, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
#else
void FCustomSceneViewExtension::SubscribeToPostProcessingPass(EPostProcessingPass Pass, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
#endif
{
	if (Pass == EPostProcessingPass::MotionBlur)
	{
#if false
		InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FCustomSceneViewExtension::CustomPostProcessFunction));
#endif
	}
}

FScreenPassTexture FCustomSceneViewExtension::CustomPostProcessFunction(FRDGBuilder& GraphBuilder, const FSceneView& SceneView, const FPostProcessMaterialInputs& Inputs)
{
	return FScreenPassTexture();
}
