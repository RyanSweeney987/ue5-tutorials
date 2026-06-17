#pragma once

#include "IDetailCustomization.h"

class IPropertyHandle;
class IPropertyUtilities;

class FExamplePluginSettingsCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	// End of IDetailCustomization interface
private:
	FReply OnIncrementExampleFloatClicked();

	TSharedPtr<IPropertyHandle> ExampleFloatProperty;
	TWeakPtr<IPropertyUtilities> PropertyUtilities;
};
