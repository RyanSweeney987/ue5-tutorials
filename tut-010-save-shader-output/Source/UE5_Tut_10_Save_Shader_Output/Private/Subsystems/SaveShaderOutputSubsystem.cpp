// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SaveShaderOutputSubsystem.h"

#include "SceneViewExtension.h"
#include "SceneViewExtensions/BlurSceneViewExtension.h"


void USaveShaderOutputSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SceneViewExtension = FSceneViewExtensions::NewExtension<FBlurSceneViewExtension>([this](TArray64<uint8>& PixelData, FIntPoint& Extent)
	{
		OnReadbackCompleteDelegate.ExecuteIfBound(PixelData, Extent);
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

void USaveShaderOutputSubsystem::QueueBlurRequest(UTexture* InTexture, const float BlurRadius, const bool bInDownloadImmediately)
{
	checkf(BlurRadius >= 0.1f && BlurRadius <= 100.0f, TEXT("BlurRadius must be between 0.1 and 100.0 inclusive"));
	SceneViewExtension->QueueBlurRequest_GameThread(InTexture, BlurRadius, bInDownloadImmediately);
}
