// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

BEGIN_SHADER_PARAMETER_STRUCT(FColourExtractParams, )
	SHADER_PARAMETER(FVector3f, TargetColour)
	SHADER_PARAMETER(float, Tolerance)
	SHADER_PARAMETER(FVector3f, DeltaWeights)

	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorTexture)
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

class FColourExtractPS : public FGlobalShader
{
public:
	DECLARE_EXPORTED_SHADER_TYPE(FColourExtractPS, Global, UE5_TUT_9_IMMEDIATE_SHADER_API);
	using FParameters = FColourExtractParams;
	SHADER_USE_PARAMETER_STRUCT(FColourExtractPS, FGlobalShader);

	class FDebugOutput : SHADER_PERMUTATION_BOOL("DEBUG_OUTPUT");
	using FPermutationDomain = TShaderPermutationDomain<FDebugOutput>;
};
