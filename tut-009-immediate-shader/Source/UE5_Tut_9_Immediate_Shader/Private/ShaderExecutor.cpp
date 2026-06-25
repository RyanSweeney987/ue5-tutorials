// Fill out your copyright notice in the Description page of Project Settings.


#include "ShaderExecutor.h"

#include "PixelShaderUtils.h"
#include "ShaderPasses/ColourExtractRenderPass.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Logging/LogMacros.h"
#include "Misc/PackageName.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "RHICommandList.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogShaderExecutor, Log, All);

namespace
{
	const TCHAR* OutputRenderTargetPackagePath = TEXT("/Game/ShaderExecutor/RT_ColourExtract");
	const TCHAR* OutputRenderTargetObjectPath = TEXT("/Game/ShaderExecutor/RT_ColourExtract.RT_ColourExtract");
}

// Sets default values
AShaderExecutor::AShaderExecutor()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShaderExecutor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShaderExecutor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShaderExecutor::RunColourExtractRenderPass()
{
#if WITH_EDITOR
	if (!GEditor)
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot execute colour extract pass: GEditor is unavailable."));
		return;
	}

	FViewport* ActiveViewport = GEditor->GetActiveViewport();
	if (!ActiveViewport)
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot execute colour extract pass: no active editor viewport."));
		return;
	}

	const FIntPoint RenderResolution = ActiveViewport->GetSizeXY();
	if (RenderResolution.X <= 0 || RenderResolution.Y <= 0)
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot execute colour extract pass: invalid viewport resolution (%d x %d)."), RenderResolution.X, RenderResolution.Y);
		return;
	}

	UTextureRenderTarget2D* OutputRenderTarget = GetOrCreateColourExtractRenderTarget(RenderResolution);
	if (!OutputRenderTarget)
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot execute colour extract pass: failed to create or load the output render target."));
		return;
	}

	ColourExtractRenderTarget = OutputRenderTarget;

	FTextureRHIRef InputSceneTexture = ActiveViewport->GetRenderTargetTexture();
	FTextureRenderTargetResource* OutputRenderTargetResource = OutputRenderTarget->GameThread_GetRenderTargetResource();
	FTextureRHIRef OutputTextureRHI = OutputRenderTargetResource ? OutputRenderTargetResource->GetRenderTargetTexture() : FTextureRHIRef();

	if (!InputSceneTexture.IsValid() || !OutputTextureRHI.IsValid())
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot execute colour extract pass: required RHI textures are not valid."));
		return;
	}

	const FVector3f TargetColourVector = FVector3f(TargetColour);
	const bool bUseUnlitSceneColourLocal = bUseUnlitSceneColour;

	ENQUEUE_RENDER_COMMAND(RunColourExtractRenderPassNow)(
		[InputSceneTexture, OutputTextureRHI, TargetColourVector, bUseUnlitSceneColourLocal](FRHICommandListImmediate& RHICmdList)
		{
			FRDGBuilder GraphBuilder(RHICmdList);

			FRDGTextureRef SceneColorTexture = GraphBuilder.RegisterExternalTexture(CreateRenderTarget(InputSceneTexture, TEXT("ShaderExecutor.SceneColor")));
			FRDGTextureRef OutputTexture = GraphBuilder.RegisterExternalTexture(CreateRenderTarget(OutputTextureRHI, TEXT("ShaderExecutor.Output")));

			// ColourExtractRenderPass::AddPass(
			// 	GraphBuilder,
			// 	GetGlobalShaderMap(GMaxRHIFeatureLevel),
			// 	SceneColorTexture,
			// 	SceneColorTexture,
			// 	OutputTexture,
			// 	TargetColourVector,
			// 	bUseUnlitSceneColourLocal);
			//
			// if (!ensure(SceneColorTexture && OutputTexture && GlobalShaderMap))
			// {
			// 	return;
			// }
			
			const FIntRect OutputViewport(FIntPoint::ZeroValue, OutputTexture->Desc.Extent);
			const FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
			
			ensure(SceneColorTexture && OutputTexture && GlobalShaderMap);

			FColourExtractPS::FPermutationDomain PermutationVector;
			PermutationVector.Set<FColourExtractPS::FUseUnlitSceneColour>(bUseUnlitSceneColourLocal);

			FColourExtractPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FColourExtractPS::FParameters>();
			PassParameters->TargetColour = TargetColourVector;
			PassParameters->SceneColorTexture = SceneColorTexture;
			// PassParameters->SceneColorUnlitTexture = SceneColorUnlitTexture ? SceneColorUnlitTexture : SceneColorTexture;
			PassParameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::ENoAction);

			TShaderMapRef<FColourExtractPS> PixelShader(GlobalShaderMap, PermutationVector);

			FPixelShaderUtils::AddFullscreenPass<FColourExtractPS>(
				GraphBuilder,
				GlobalShaderMap,
				RDG_EVENT_NAME("ColourExtractPass"),
				PixelShader,
				PassParameters,
				OutputViewport);

			GraphBuilder.Execute();
		});

	FlushRenderingCommands();
#else
	UE_LOG(LogShaderExecutor, Warning, TEXT("RunColourExtractRenderPass is editor-only."));
#endif
}

#if WITH_EDITOR
UTextureRenderTarget2D* AShaderExecutor::GetOrCreateColourExtractRenderTarget(const FIntPoint& RenderResolution)
{
	UTextureRenderTarget2D* RenderTarget = LoadObject<UTextureRenderTarget2D>(nullptr, OutputRenderTargetObjectPath);
	if (!RenderTarget)
	{
		const FString PackageName = FString(OutputRenderTargetPackagePath);
		const FString AssetName = FPackageName::GetLongPackageAssetName(PackageName);
		UPackage* Package = CreatePackage(*PackageName);

		RenderTarget = NewObject<UTextureRenderTarget2D>(Package, *AssetName, RF_Public | RF_Standalone);
		if (!RenderTarget)
		{
			UE_LOG(LogShaderExecutor, Error, TEXT("Failed to allocate render target asset '%s'."), OutputRenderTargetObjectPath);
			return nullptr;
		}

		RenderTarget->RenderTargetFormat = RTF_RGBA16f;
		RenderTarget->ClearColor = FLinearColor::Black;
		RenderTarget->AddressX = TA_Clamp;
		RenderTarget->AddressY = TA_Clamp;
		RenderTarget->bAutoGenerateMips = false;
		RenderTarget->InitAutoFormat(RenderResolution.X, RenderResolution.Y);
		RenderTarget->UpdateResourceImmediate(true);

		FAssetRegistryModule::AssetCreated(RenderTarget);
		Package->MarkPackageDirty();
	}
	else if (RenderTarget->SizeX != RenderResolution.X || RenderTarget->SizeY != RenderResolution.Y)
	{
		RenderTarget->ResizeTarget(RenderResolution.X, RenderResolution.Y);
	}

	if (!SaveColourExtractRenderTarget(RenderTarget))
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Failed to save render target asset '%s'."), OutputRenderTargetObjectPath);
		return nullptr;
	}

	return RenderTarget;
}

bool AShaderExecutor::SaveColourExtractRenderTarget(UTextureRenderTarget2D* RenderTarget) const
{
	if (!RenderTarget)
	{
		return false;
	}

	UPackage* Package = RenderTarget->GetOutermost();
	if (!Package)
	{
		return false;
	}

	Package->MarkPackageDirty();

	const FString PackageName = Package->GetName();
	const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.SaveFlags = SAVE_None;
	return UPackage::SavePackage(Package, RenderTarget, *PackageFileName, SaveArgs);
}
#endif
