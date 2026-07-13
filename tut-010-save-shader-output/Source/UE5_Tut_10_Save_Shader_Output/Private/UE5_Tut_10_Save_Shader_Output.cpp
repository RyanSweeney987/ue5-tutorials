// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_Tut_10_Static_Texture_In_Shader.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

#define LOCTEXT_NAMESPACE "FUE5_Tut_10_Static_Texture_In_ShaderModule"

void FUE5_Tut_10_Static_Texture_In_ShaderModule::StartupModule()
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("UE5_Tut_10_Static_Texture_In_Shader"));
	if (Plugin.IsValid())
	{
		const FString ShaderDirectory = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/Plugin/UE5_Tut_10_Static_Texture_In_Shader"), ShaderDirectory);
	}
}

void FUE5_Tut_10_Static_Texture_In_ShaderModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE5_Tut_10_Static_Texture_In_ShaderModule, UE5_Tut_10_Static_Texture_In_Shader)