#pragma once

#include "Modules/ModuleManager.h"

class FUE5_Tut_7_Editor_Customisations_SettingsEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
