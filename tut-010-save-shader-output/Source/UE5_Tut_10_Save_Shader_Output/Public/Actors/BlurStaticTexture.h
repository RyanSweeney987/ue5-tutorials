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

	UPROPERTY(EditAnywhere, Category= "Blur", meta=(ClampMin="0.1", UIMin="0.1", ClampMax="100.0", UIMax="100.0"))
	float BlurRadius = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Blur")
	TObjectPtr<UTexture2D> SourceTexture;

	
	UPROPERTY(EditAnywhere, Category = "Blur")
	FString OutputAssetPath = TEXT("/Game");

	UPROPERTY(EditAnywhere, Category = "Blur")
	FString OutputAssetNamePrefix = TEXT("T_BlurResult");

	UPROPERTY(EditAnywhere, Category = "Blur")
	bool bDownloadImmediately = false;

	UFUNCTION(CallInEditor, Category = "Blur", DisplayName = "Run Blur")
	void RunBlur();

private:
	ETextureSourceFormat SourceFormat;
	EPixelFormat SourcePixelFormat;
	
#if WITH_EDITOR
	void SaveTextureAssetFromReadback(const TArray64<uint8>& PixelData, const FIntPoint& Extent);
#endif

};
