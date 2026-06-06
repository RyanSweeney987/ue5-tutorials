#include "ExamplePluginSettingsCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "ExamplePluginSettings.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SButton.h"

#ifdef LOCTEXT_NAMESPACE
#undef LOCTEXT_NAMESPACE
#endif

#define LOCTEXT_NAMESPACE "FExamplePluginSettingsCustomization"

TSharedRef<IDetailCustomization> FExamplePluginSettingsCustomization::MakeInstance()
{
	return MakeShared<FExamplePluginSettingsCustomization>();
}

void FExamplePluginSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// Set the property handle for the ExampleFloat property so we can modify it when the button is clicked
	ExampleFloatProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UExamplePluginSettings, ExampleFloat));

	// We hide and then re-add later to force the ordering of the property and button so that the button appears directly below the property
	DetailBuilder.HideProperty(ExampleFloatProperty);

	// Category is the same value used on the UPROPERTY
	IDetailCategoryBuilder& TypesCategory = DetailBuilder.EditCategory(TEXT("Types"));
	// Re-add the property with a custom widget
	TypesCategory.AddProperty(ExampleFloatProperty.ToSharedRef());
	TypesCategory.AddCustomRow(LOCTEXT("IncrementExampleFloatFilter", "Increment Example Float"))
	// To have the content fill the entire row use .WholeRowContent() or just use []
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("NameContentExample", "Name Content Example"))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	]
	.ValueContent()
	[
		// Use slate to create a button that will increment the ExampleFloat value when clicked
		// Without the horizontal box, the button will be stretched across the row
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.Text(LOCTEXT("IncrementExampleFloatButton", "Increment Example Float"))
			.OnClicked(this, &FExamplePluginSettingsCustomization::OnIncrementExampleFloatClicked)
		]
	];
}

FReply FExamplePluginSettingsCustomization::OnIncrementExampleFloatClicked()
{
	UExamplePluginSettings* Settings = UExamplePluginSettings::Get();
	// Make sure the settings and property are valid before trying to change the value
	if (!ensure(Settings != nullptr) || !ensure(ExampleFloatProperty.IsValid()))
	{
		return FReply::Handled();
	}

	// Change the settings object and the property handle to update the value and notify the details panel of the change
	ExampleFloatProperty->NotifyPreChange();
	Settings->Modify();
	Settings->SetExampleFloat(Settings->ExampleFloat + 1.0f);
	ExampleFloatProperty->NotifyPostChange(EPropertyChangeType::ValueSet);
	ExampleFloatProperty->NotifyFinishedChangingProperties();

	// Refresh the details panel to show the new value
	if (const TSharedPtr<IPropertyUtilities> PinnedPropertyUtilities = PropertyUtilities.Pin())
	{
		PinnedPropertyUtilities->ForceRefresh();
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
