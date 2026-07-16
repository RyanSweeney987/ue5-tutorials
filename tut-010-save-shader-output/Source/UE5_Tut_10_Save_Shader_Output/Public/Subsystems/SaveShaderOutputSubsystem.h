// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "SaveShaderOutputSubsystem.generated.h"

class FBlurSceneViewExtension;
/**
 * 
 */
UCLASS()
class UE5_TUT_10_SAVE_SHADER_OUTPUT_API USaveShaderOutputSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
	TSharedPtr<FBlurSceneViewExtension, ESPMode::ThreadSafe> SceneViewExtension;
	
	DECLARE_DELEGATE_TwoParams(FOnReadbackComplete, const TArray64<float>&, const FIntPoint& Extent);
	FOnReadbackComplete OnReadbackCompleteDelegate;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	static USaveShaderOutputSubsystem* Get();
		
	void QueueBlurRequest(UTexture* InTexture, const float BlurRadius, const bool bInDownloadImmediately = false);
	
	FOnReadbackComplete& OnReadbackComplete() { return OnReadbackCompleteDelegate; }
};
