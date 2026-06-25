// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShaderExecutor.generated.h"

class UTextureRenderTarget2D;

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

	UFUNCTION(CallInEditor, Category = "Shader Executor")
	void RunColourExtractRenderPass();

	UPROPERTY(EditAnywhere, Category = "Shader Executor")
	FLinearColor TargetColour = FLinearColor::Red;

	UPROPERTY(EditAnywhere, Category = "Shader Executor")
	bool bUseUnlitSceneColour = false;

	UPROPERTY(VisibleAnywhere, Category = "Shader Executor")
	TObjectPtr<UTextureRenderTarget2D> ColourExtractRenderTarget = nullptr;

#if WITH_EDITOR
private:
	UTextureRenderTarget2D* GetOrCreateColourExtractRenderTarget(const FIntPoint& RenderResolution);
	bool SaveColourExtractRenderTarget(UTextureRenderTarget2D* RenderTarget) const;
#endif
};
