#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

class UE5_TUT_10_STATIC_TEXTURE_IN_SHADER_API FBlurPS : public FGlobalShader
{
public:
	BEGIN_SHADER_PARAMETER_STRUCT(FBlurPSParam, )
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D, InputTexture)
		SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler)
		SHADER_PARAMETER(FVector2f, TexelSize)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()
	
	DECLARE_EXPORTED_SHADER_TYPE(FBlurPS, Global, );
	using FParameters = FBlurPSParam;
	SHADER_USE_PARAMETER_STRUCT(FBlurPS, FGlobalShader);
};
