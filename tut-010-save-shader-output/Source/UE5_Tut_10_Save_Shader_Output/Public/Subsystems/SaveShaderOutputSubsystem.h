// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/BlurRequests.h"
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
	
private:
	TOptional<FBlurRequestData> CurrentBlurRequest;
	
	TSharedPtr<FBlurSceneViewExtension, ESPMode::ThreadSafe> SceneViewExtension;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	static USaveShaderOutputSubsystem* Get();
		
	void QueueBlurRequest(const FBlurRequestData& BlurRequestData);
#if WITH_EDITOR
	void SaveTextureAssetFromReadback(const TArray64<float>& PixelData, const FIntPoint& Extent);
#endif
};
