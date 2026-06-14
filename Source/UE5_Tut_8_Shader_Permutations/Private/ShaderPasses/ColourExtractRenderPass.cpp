#include "ShaderPasses/ColourExtractRenderPass.h"

#include "SceneRenderTargetParameters.h"
#include "SceneTexturesConfig.h"

IMPLEMENT_SHADER_TYPE(, FColourExtractPS, TEXT("/ShaderPermutations/private/TutorialShader.usf"), TEXT("MainPS"), SF_Pixel);
