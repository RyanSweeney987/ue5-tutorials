#include "UE5_Tut_7_Editor_Customisations_SettingsEditor.h"

#include "ExamplePluginSettings.h"
#include "ExamplePluginSettingsCustomization.h"
#include "PropertyEditorModule.h"

void FUE5_Tut_7_Editor_Customisations_SettingsEditorModule::StartupModule()
{
	// Register the details customisation for our settings object
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout(UExamplePluginSettings::StaticClass()->GetFName(), 
		FOnGetDetailCustomizationInstance::CreateStatic(&FExamplePluginSettingsCustomization::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FUE5_Tut_7_Editor_Customisations_SettingsEditorModule::ShutdownModule()
{
	// Unregister the details customisation for our settings object
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyEditorModule.UnregisterCustomClassLayout(UExamplePluginSettings::StaticClass()->GetFName());
		PropertyEditorModule.NotifyCustomizationModuleChanged();
	}
}

IMPLEMENT_MODULE(FUE5_Tut_7_Editor_Customisations_SettingsEditorModule, UE5_Tut_7_Editor_Customisations_SettingsEditor)
