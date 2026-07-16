#include "Actors/BlurStaticTexture.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Engine/Texture2D.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "RHICommandList.h"
#include "SceneViewExtension.h"
#include "Factories/Texture2dFactoryNew.h"
#include "SceneViewExtensions/BlurSceneViewExtension.h"
#include "Subsystems/SaveShaderOutputSubsystem.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"

ABlurStaticTexture::ABlurStaticTexture()
{
	PrimaryActorTick.bCanEverTick = false;
	
	if(USaveShaderOutputSubsystem* Subsystem = USaveShaderOutputSubsystem::Get())
	{
		Subsystem->OnReadbackComplete().BindUObject(this, &ABlurStaticTexture::SaveTextureAssetFromReadback);
	}
}

/**
 * Offloads the work to a scene view extension
 */
void ABlurStaticTexture::RunBlur()
{
	if(!SourceTexture)
	{	
		UE_LOG(LogTemp, Warning, TEXT("BlurStaticTexture: SourceTexture is not set."));
		return;
	}
	
	if(USaveShaderOutputSubsystem* Subsystem = USaveShaderOutputSubsystem::Get())
	{
		// CPU/source format (for NewTexture->Source.Init)
		SourceFormat = SourceTexture->Source.GetFormat();
		// Runtime GPU format (useful for readback assumptions)
		SourcePixelFormat = SourceTexture->GetPixelFormat(); 
		
		Subsystem->QueueBlurRequest(SourceTexture.Get(), BlurRadius, bDownloadImmediately);
	}
}

#if WITH_EDITOR
/**
 * NOTE: This code works with textures that are set to the RGBA8 format or Compression Settings set to UserInterface2D
 * @param PixelData 
 * @param Extent 
 */
void ABlurStaticTexture::SaveTextureAssetFromReadback(const TArray64<float>& PixelData, const FIntPoint& Extent)
{
	if (PixelData.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("BlurStaticTexture: Invalid readback data."));
		return;
	}

	// Get path
	FString AssetPath = OutputAssetPath;
	if (!AssetPath.StartsWith(TEXT("/Game")))
	{
		AssetPath = TEXT("/Game");
	}
	AssetPath.RemoveFromEnd(TEXT("/"));

	// Get name 
	const FString NamePrefix = OutputAssetNamePrefix.IsEmpty() ? TEXT("T_BlurResult") : OutputAssetNamePrefix;
	const FString BasePackageName = FString::Printf(TEXT("%s/%s"), *AssetPath, *NamePrefix);

	FString UniquePackageName;
	FString UniqueAssetName;
	// Make sure asset name is unique
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().CreateUniqueAssetName(BasePackageName, TEXT(""), UniquePackageName, UniqueAssetName);

	UPackage* Package = CreatePackage(*UniquePackageName);
	if (!Package)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlurStaticTexture: Failed to create package %s"), *UniquePackageName);
		return;
	}

	UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *UniqueAssetName, RF_Public | RF_Standalone);
	if (!NewTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlurStaticTexture: Failed to allocate output texture."));
		return;
	}
	
	NewTexture->MipGenSettings = TMGS_NoMipmaps;
	NewTexture->CompressionSettings = TC_HDR_F32;
	NewTexture->SRGB = false;
	NewTexture->Source.Init(Extent.X, Extent.Y, 1, 1, 
		TSF_RGBA32F, reinterpret_cast<const uint8*>(PixelData.GetData()));
	NewTexture->UpdateResource();

	FAssetRegistryModule::AssetCreated(NewTexture);
	Package->MarkPackageDirty();

	const FString PackageFilename = FPackageName::LongPackageNameToFilename(UniquePackageName, FPackageName::GetAssetPackageExtension());
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	if (!UPackage::SavePackage(Package, NewTexture, *PackageFilename, SaveArgs))
	{
		UE_LOG(LogTemp, Warning, TEXT("BlurStaticTexture: Failed to save package %s"), *PackageFilename);
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("BlurStaticTexture: Saved blurred texture '%s'"), *UniquePackageName);
}
#endif
