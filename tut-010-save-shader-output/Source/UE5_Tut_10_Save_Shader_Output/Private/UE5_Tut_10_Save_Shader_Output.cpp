// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_Tut_10_Save_Shader_Output.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

	#define LOCTEXT_NAMESPACE "FUE5_Tut_10_Save_Shader_OutputModule"

void FUE5_Tut_10_Save_Shader_OutputModule::StartupModule()
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("UE5_Tut_10_Save_Shader_Output"));
	if (Plugin.IsValid())
	{
		const FString ShaderDirectory = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/Plugin/BlurShader"), ShaderDirectory);
	}
}

void FUE5_Tut_10_Save_Shader_OutputModule	::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE5_Tut_10_Save_Shader_OutputModule, UE5_Tut_10_Save_Shader_Output)