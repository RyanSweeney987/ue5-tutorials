#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlurStaticTexture.generated.h"

class FBlurSceneViewExtension;
class UTexture;

UCLASS()
class UE5_TUT_10_SAVE_SHADER_OUTPUT_API ABlurStaticTexture : public AActor
{
	GENERATED_BODY()

public:
	ABlurStaticTexture();

	UPROPERTY(EditAnywhere, Category = "Blur")
	TObjectPtr<UTexture> SourceTexture;

	UPROPERTY(EditAnywhere, Category = "Blur")
	FString OutputAssetPath = TEXT("/Game/BlurOutputs");

	UPROPERTY(EditAnywhere, Category = "Blur")
	FString OutputAssetNamePrefix = TEXT("T_BlurResult");

	UPROPERTY(EditAnywhere, Category = "Blur")
	bool bDownloadImmediately = false;

	UFUNCTION(CallInEditor, Category = "Blur", DisplayName = "Run Blur and Save Texture")
	void RunBlurAndSaveTexture();

protected:
	virtual void BeginDestroy() override;

private:
	void EnsureSceneViewExtension();
	void EnqueueBlurRequest(TFunction<void(TArray64<uint8>&&, int32, int32)> InCallback, bool bInDownloadImmediately);

#if WITH_EDITOR
	void SaveTextureAssetFromReadback(TArray64<uint8>&& PixelData, int32 Width, int32 Height);
#endif

	TSharedPtr<FBlurSceneViewExtension, ESPMode::ThreadSafe> BlurSceneViewExtension;
};
