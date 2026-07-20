// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SaveShaderOutputSubsystem.h"

#include "AssetToolsModule.h"
#include "SceneViewExtension.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Data/BlurRequests.h"
#include "SceneViewExtensions/BlurSceneViewExtension.h"
#include "UObject/SavePackage.h"


void USaveShaderOutputSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	SceneViewExtension = FSceneViewExtensions::NewExtension<FBlurSceneViewExtension>();
	SceneViewExtension->OnBlurRequestCompleted().BindLambda([this](const TArray64<float>& PixelData, const FIntPoint& Extent)
	{
#if WITH_EDITOR
		// This is an editor only function
		// So this takes the results from the SVE and process it here
		SaveTextureAssetFromReadback(PixelData, Extent);
#else
		UE_LOG(LogTemp, Warning, TEXT("SaveShaderOutputSubsystem: SaveTextureAssetFromReadback is only available in the editor."));
#endif
		
		// Once we're done, clear the blur request so we can add a new one
		CurrentBlurRequest.Reset();
	});
	
}

void USaveShaderOutputSubsystem::Deinitialize()
{
	SceneViewExtension.Reset();
}

USaveShaderOutputSubsystem* USaveShaderOutputSubsystem::Get()
{
	if(GEngine)
	{
		return GEngine->GetEngineSubsystem<USaveShaderOutputSubsystem>();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Save shader output subsystem is not available."));
	return nullptr;
}

void USaveShaderOutputSubsystem::QueueBlurRequest(const FBlurRequestData& BlurRequestData)
{
	checkf(BlurRequestData.BlurRadius >= 0.1f && BlurRequestData.BlurRadius <= 1000.0f, TEXT("BlurRadius must be between 0.1 and 100.0 inclusive"));
	
	if(CurrentBlurRequest.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Blur request is already in progress. New request will be ignored."));
		return;
	}
	
	CurrentBlurRequest = BlurRequestData;
	SceneViewExtension->QueueBlurRequest_GameThread(BlurRequestData);
}

#if WITH_EDITOR
void USaveShaderOutputSubsystem::SaveTextureAssetFromReadback(const TArray64<float>& PixelData, const FIntPoint& Extent)
{
	checkf(CurrentBlurRequest.IsSet(), TEXT("No current blur request is set. This function should only be called after a blur request has been processed."));
	
	if (PixelData.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("BlurStaticTexture: Invalid readback data."));
		return;
	}

	// Get path
	FString AssetPath = CurrentBlurRequest->OutputAssetPath;
	if (!AssetPath.StartsWith(TEXT("/Game")))
	{
		AssetPath = TEXT("/Game");
	}
	AssetPath.RemoveFromEnd(TEXT("/"));

	// Get name 
	const FString NamePrefix = CurrentBlurRequest->OutputAssetNamePrefix.IsEmpty() ? TEXT("T_BlurResult") : CurrentBlurRequest->OutputAssetNamePrefix;
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