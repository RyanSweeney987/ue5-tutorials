#pragma once

#include "CoreMinimal.h"
#include "SceneTexturesConfig.h"
#include "ShaderParameterStruct.h"

BEGIN_SHADER_PARAMETER_STRUCT(FColourExtractParams,)
	SHADER_PARAMETER(FVector3f, TargetColour)
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorTexture)
	SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
	SHADER_PARAMETER_STRUCT_INCLUDE(FSceneTextureShaderParameters, SceneTextures)
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

enum class EMyEnum
{
		Value1 = 0,
		Value2 = 1,
		Value3 = 2,
		MAX = 3
};

class FColourExtractPS : public FGlobalShader
{
	DECLARE_EXPORTED_SHADER_TYPE(FColourExtractPS, Global, );
	using FParameters = FColourExtractParams;
	SHADER_USE_PARAMETER_STRUCT(FColourExtractPS, FGlobalShader);
	
	// Create classes for the type of permutations
	class FBoolPermutation : SHADER_PERMUTATION_BOOL("BOOL_DEFINE");
	class FIntPermutation : SHADER_PERMUTATION_INT("INT_DEFINE", 5);
	class FIntRangePermutation : SHADER_PERMUTATION_RANGE_INT("INT_DEFINE", 2, 3);
	class FSparseIntPermutation : SHADER_PERMUTATION_SPARSE_INT("SPARSE_INT_DEFINE", 16, 32, 64, 128, 256, 512);
	class FEnumPermutation : SHADER_PERMUTATION_ENUM_CLASS("ENUM_DEFINE", EMyEnum);
	// Create a permutation domain that combines the permutations
	// There is an upper limit to the permutations set in the engine code
	using FPermutationDomain = TShaderPermutationDomain<FBoolPermutation, FIntPermutation, FIntRangePermutation, FSparseIntPermutation, FEnumPermutation>;
		
	// The following macros can be used for the permutations and are found in ShaderPermutation.h
	// SHADER_PERMUTATION_BOOL("BOOL_DEFINE")
	// SHADER_PERMUTATION_INT("INT_DEFINE", 2, 3)
	// SHADER_PERMUTATION_RANGE_INT("INT_DEFINE", 2, 3)
	// SHADER_PERMUTATION_SPARSE_INT("SPARSE_INT_DEFINE", 16, 32, 64, 128, 256, 512)
	// SHADER_PERMUTATION_ENUM_CLASS("ENUM_DEFINE", EMyEnum, Value1, Value2, Value3)

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		
		// Do further processing on the permutation values by getting the permutation domain
		const FPermutationDomain PermutationVector(Parameters.PermutationId);
		// And extracting the values for each permutation
		const bool BoolValue = PermutationVector.Get<FBoolPermutation>();
		// Set the define based on the bool permutation value
		OutEnvironment.SetDefine(TEXT("USE_UNLIT_SCENE_COLOUR"), BoolValue);
		// Normally you would do something like the following and add it to the TShaderPermutationDomain<...> list above
		// class FUnlitScenePermutation : SHADER_PERMUTATION_BOOL("USE_UNLIT_SCENE_COLOUR");		
	}
};