// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_Tut_8_Shader_Permutations.h"

#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FUE5_Tut_8_Shader_PermutationsModule"

void FUE5_Tut_8_Shader_PermutationsModule::StartupModule()
{
	const FString PluginShaderDir = FPaths::Combine(
		IPluginManager::Get().FindPlugin(TEXT("UE5_Tut_8_Shader_Permutations"))->GetBaseDir(),
		TEXT("Shaders"));

	if (!AllShaderSourceDirectoryMappings().Contains(TEXT("/ShaderPermutations")))
	{
		AddShaderSourceDirectoryMapping(TEXT("/ShaderPermutations"), PluginShaderDir);
	}
}

void FUE5_Tut_8_Shader_PermutationsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE5_Tut_8_Shader_PermutationsModule, UE5_Tut_8_Shader_Permutations)
