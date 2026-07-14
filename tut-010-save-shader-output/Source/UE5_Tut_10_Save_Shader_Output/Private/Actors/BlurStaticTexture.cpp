#include "Actors/BlurStaticTexture.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Engine/Texture2D.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "RHICommandList.h"
#include "SceneViewExtension.h"
#include "SceneViewExtensions/BlurSceneViewExtension.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"

ABlurStaticTexture::ABlurStaticTexture()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABlurStaticTexture::RunBlurAndSaveTexture()
{
	if (!SourceTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlurStaticTexture: SourceTexture is not set."));
		return;
	}
	
	TWeakObjectPtr<ABlurStaticTexture> WeakThis(this);
	EnqueueBlurRequest(
		[WeakThis](TArray64<uint8>&& PixelData, int32 Width, int32 Height) mutable
		{
			if (!WeakThis.IsValid())
			{
				return;
			}

#if WITH_EDITOR
			WeakThis->SaveTextureAssetFromReadback(MoveTemp(PixelData), Width, Height);
#endif
		},
		bDownloadImmediately);
}

void ABlurStaticTexture::EnqueueBlurRequest(
	TFunction<void(TArray64<uint8>&&, int32, int32)> InCallback,
	const bool bInDownloadImmediately)
{
	if (!BlurSceneViewExtension.IsValid())
	{
		return;
	}

	TSharedPtr<FBlurSceneViewExtension, ESPMode::ThreadSafe> LocalExtension = BlurSceneViewExtension;
	ENQUEUE_RENDER_COMMAND(QueueBlurSceneViewRequest)(
		[LocalExtension, Callback = MoveTemp(InCallback), bInDownloadImmediately](FRHICommandListImmediate& RHICmdList) mutable
		{
			(void)RHICmdList;

			if (!LocalExtension.IsValid())
			{
				return;
			}

			LocalExtension->QueueBlurRequest_RenderThread(MoveTemp(Callback), bInDownloadImmediately);
		});
}

#if WITH_EDITOR
void ABlurStaticTexture::SaveTextureAssetFromReadback(TArray64<uint8>&& PixelData, int32 Width, int32 Height)
{
	if (PixelData.IsEmpty() || Width <= 0 || Height <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlurStaticTexture: Invalid readback data."));
		return;
	}

	FString AssetPath = OutputAssetPath;
	if (!AssetPath.StartsWith(TEXT("/Game")))
	{
		AssetPath = TEXT("/Game/BlurOutputs");
	}
	AssetPath.RemoveFromEnd(TEXT("/"));

	const FString NamePrefix = OutputAssetNamePrefix.IsEmpty() ? TEXT("T_BlurResult") : OutputAssetNamePrefix;
	const FString BasePackageName = FString::Printf(TEXT("%s/%s"), *AssetPath, *NamePrefix);

	FString UniquePackageName;
	FString UniqueAssetName;
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
	NewTexture->CompressionSettings = TC_Default;
	NewTexture->SRGB = true;
	NewTexture->Source.Init(Width, Height, 1, 1, TSF_RGBA16F, PixelData.GetData());
	NewTexture->UpdateResource();

	FAssetRegistryModule::AssetCreated(NewTexture);
	Package->MarkPackageDirty();

	const FString PackageFilename =
		FPackageName::LongPackageNameToFilename(UniquePackageName, FPackageName::GetAssetPackageExtension());
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
