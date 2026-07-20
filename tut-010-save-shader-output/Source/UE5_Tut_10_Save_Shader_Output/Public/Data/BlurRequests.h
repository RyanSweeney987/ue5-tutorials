#pragma once

#include "CoreMinimal.h"

struct FBlurRequestData
{
	// Source
	UTexture2D* Texture = nullptr;
	// Blur pass settings
	bool bInDownloadImmediately = false;
	float BlurRadius = 1.0f;
	// Source info
	ETextureSourceFormat SourceFormat = TSF_Invalid;
	EPixelFormat SourcePixelFormat = PF_Unknown;
	// Output info
	FString OutputAssetPath = TEXT("/Game");
	FString OutputAssetNamePrefix = TEXT("T_BlurResult");
	
	FBlurRequestData()
		: Texture(nullptr)
		, SourceFormat(TSF_Invalid)
		, SourcePixelFormat(PF_Unknown)
	{}
	
	FBlurRequestData(UTexture2D* InTexture, const float InBlurRadius, const bool bInDownloadImmediately = false)
		: Texture(InTexture)
		, bInDownloadImmediately(bInDownloadImmediately)
		, BlurRadius(InBlurRadius)
	{
		if (Texture)
		{
			SourceFormat = Texture->Source.GetFormat();
			SourcePixelFormat = Texture->GetPixelFormat();
		}
	}
	
	FBlurRequestData(UTexture2D* InTexture, const float InBlurRadius, const bool bInDownloadImmediately, const FString& InOutputAssetPath, const FString& InOutputAssetNamePrefix)
		: Texture(InTexture)
		, bInDownloadImmediately(bInDownloadImmediately)
		, BlurRadius(InBlurRadius)
		, OutputAssetPath(InOutputAssetPath)
		, OutputAssetNamePrefix(InOutputAssetNamePrefix)
	{
		if (Texture)
		{
			SourceFormat = Texture->Source.GetFormat();
			SourcePixelFormat = Texture->GetPixelFormat();
		}
	}
};