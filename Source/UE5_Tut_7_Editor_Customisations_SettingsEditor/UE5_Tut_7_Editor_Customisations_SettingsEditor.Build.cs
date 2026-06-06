using UnrealBuildTool;

public class UE5_Tut_7_Editor_Customisations_SettingsEditor : ModuleRules
{
	public UE5_Tut_7_Editor_Customisations_SettingsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"PropertyEditor",
				"Slate",
				"SlateCore",
				"UE5_Tut_7_Editor_Customisations_Settings",
			}
		);
	}
}
