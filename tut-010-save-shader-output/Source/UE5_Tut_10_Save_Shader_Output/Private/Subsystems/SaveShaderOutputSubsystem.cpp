// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SaveShaderOutputSubsystem.h"

#include "SceneViewExtension.h"


void USaveShaderOutputSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SceneViewExtension = FSceneViewExtensions::NewExtension<FBlurSceneViewExtension>();
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

void USaveShaderOutputSubsystem::SetSourceTexture_GameThread(UTexture2D* InTexture)
{
	FScopeLock Lock(&SourceTextureMutex);
	SourceTexture = InTexture;
}

/**
 * We consume it so that in the scene view extension, we use it once
 * It should still be valid as it's stored in the actors TObjectPtr
 * @return the source texture
 */
UTexture2D* USaveShaderOutputSubsystem::ConsumeSourceTexture_RenderThread()
{
	FScopeLock Lock(&SourceTextureMutex);
	
	UTexture2D* ConsumedTexture = SourceTexture.Get();
	SourceTexture.Reset();
	
	return ConsumedTexture;
}

bool USaveShaderOutputSubsystem::IsSourceTextureSet() const
{
	return SourceTexture.IsValid();
}
