// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UE5_Tut_10_Static_Texture_In_Shader : ModuleRules
{
	public UE5_Tut_10_Static_Texture_In_Shader(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"AssetRegistry",
					"AssetTools",
					"UnrealEd"
				}
			);
		}
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"RHI",
				"RenderCore",
				"Renderer"
			}
		);
	}
}
