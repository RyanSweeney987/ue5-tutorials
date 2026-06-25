// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "RenderGraphFwd.h"
#include "ShaderParameterStruct.h"

BEGIN_SHADER_PARAMETER_STRUCT(FColourExtractParams, )
	SHADER_PARAMETER(FVector3f, TargetColour)
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorTexture)
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

class FColourExtractPS : public FGlobalShader
{
public:
	DECLARE_EXPORTED_SHADER_TYPE(FColourExtractPS, Global, );
	using FParameters = FColourExtractParams;
	SHADER_USE_PARAMETER_STRUCT(FColourExtractPS, FGlobalShader);
	
	class FUseUnlitSceneColour : SHADER_PERMUTATION_BOOL("USE_UNLIT_SCENE_COLOUR");
	using FPermutationDomain = TShaderPermutationDomain<FUseUnlitSceneColour>;
	using FParameters = FColourExtractParams;

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	}
};