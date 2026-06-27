// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Actor.h"
#include "ShaderExecutor.generated.h"

class USceneCaptureComponent2D;

UCLASS()
class UE5_TUT_9_IMMEDIATE_SHADER_API AShaderExecutor : public AActor
{
	GENERATED_BODY()

public:
	AShaderExecutor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor, Category = "Shader Executor", meta = (DisplayName = "Execute Shader"))
	void RunColourExtractRenderPass();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shader Executor")
	bool bDebugOutput = false;

	UPROPERTY(EditAnywhere, Category = "Shader Executor")
	FLinearColor TargetColour = FLinearColor::Red;
	
	UPROPERTY(EditAnywhere, Category = "Shader Executor")
	float Tolerance = 45.0f;
	
	UPROPERTY(EditAnywhere, Category = "Shader Executor")
	FVector3f DeltaWeights = FVector3f(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Shader Executor", meta = (ClampMin = "1", UIMin = "1"))
	FIntPoint CaptureResolution = FIntPoint(1920, 1080);

	UPROPERTY(VisibleAnywhere, Category = "Shader Executor")
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Shader Executor")
	TObjectPtr<UTextureRenderTarget2D> SceneColorCaptureTarget = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Shader Executor")
	TObjectPtr<UTextureRenderTarget2D> ColourExtractRenderTarget = nullptr;

private:
#if WITH_EDITOR
	UTextureRenderTarget2D* GetOrCreateColourExtractRenderTarget(const TCHAR* ObjectPath, const FIntPoint& Resolution, 
		ETextureRenderTargetFormat Format);
	bool SaveColourExtractRenderTarget(UTextureRenderTarget2D* RenderTarget) const;
#endif
};
