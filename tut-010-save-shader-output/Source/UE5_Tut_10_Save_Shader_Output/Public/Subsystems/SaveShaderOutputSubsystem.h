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
	
	FCriticalSection SourceTextureMutex;
	TWeakObjectPtr<UTexture2D> SourceTexture;
	
	DECLARE_DELEGATE_OneParam(FOnReadbackComplete, const TArray64<uint8>&);
	FOnReadbackComplete OnReadbackCompleteDelegate;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	static USaveShaderOutputSubsystem* Get();
	
	void SetSourceTexture_GameThread(UTexture2D* InTexture);
	UTexture2D* ConsumeSourceTexture_RenderThread();
	
	bool IsSourceTextureSet() const;
	
	FOnReadbackComplete& OnReadbackComplete() { return OnReadbackCompleteDelegate; }
};
