// Fill out your copyright notice in the Description page of Project Settings.

#include "ShaderExecutor.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SceneComponent.h"
#include "Editor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Logging/LogMacros.h"
#include "Misc/PackageName.h"
#include "PixelShaderUtils.h"
#include "RHICommandList.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "ShaderPasses/ColourExtractRenderPass.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"

// For outputting to the log
DEFINE_LOG_CATEGORY_STATIC(LogShaderExecutor, Log, All);

namespace
{
	const TCHAR* SceneColorTargetPath = TEXT("/Game/ShaderExecutor/RT_SceneColor.RT_SceneColor");
	const TCHAR* OutputTargetPath = TEXT("/Game/ShaderExecutor/RT_ColourExtract.RT_ColourExtract");
}

AShaderExecutor::AShaderExecutor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComponent);

	// Create the scene capture component so we can capture the scene color buffer
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(RootComponent);
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;
	SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
}

void AShaderExecutor::BeginPlay()
{
	Super::BeginPlay();
}

void AShaderExecutor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShaderExecutor::RunColourExtractRenderPass()
{
#if WITH_EDITOR
	if (!GEditor)
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot run colour extract pass: editor is unavailable."));
		return;
	}

	if (!SceneCaptureComponent)
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot run colour extract pass: scene capture component is missing."));
		return;
	}

	if (CaptureResolution.X <= 0 || CaptureResolution.Y <= 0)
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot run colour extract pass: invalid capture resolution (%d x %d)."), 
			CaptureResolution.X, CaptureResolution.Y);
		return;
	}

	// Get the existing render target assets or create new ones if none exist
	SceneColorCaptureTarget = GetOrCreateColourExtractRenderTarget(SceneColorTargetPath, CaptureResolution, RTF_RGBA16f);
	ColourExtractRenderTarget = GetOrCreateColourExtractRenderTarget(OutputTargetPath, CaptureResolution, RTF_RGBA16f);

	if (!SceneColorCaptureTarget || !ColourExtractRenderTarget)
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot run colour extract pass: failed to prepare render targets."));
		return;
	}
	
	// Update the scene capture component
	SceneCaptureComponent->SetWorldTransform(GetActorTransform());
	
    // Set up the capture then immediately execute the render commands
    SceneCaptureComponent->TextureTarget = SceneColorCaptureTarget;
    SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
    SceneCaptureComponent->CaptureScene();
	// Flush here so we can get the scene capture results immediately for use later on
	// This will hold up the game thread until the render thread has finished executing the capture commands
    FlushRenderingCommands();

	// Get the underlying resources so we can get the RHI texture references for the render pass.
	const FTextureRenderTargetResource* SceneColorResource = SceneColorCaptureTarget->GameThread_GetRenderTargetResource();
	const FTextureRenderTargetResource* OutputResource = ColourExtractRenderTarget->GameThread_GetRenderTargetResource();

	// Get the RHIs to create RDG render targets
	const FTextureRHIRef SceneColorTextureRHI = SceneColorResource ? SceneColorResource->GetRenderTargetTexture() : FTextureRHIRef();
	const FTextureRHIRef OutputTextureRHI = OutputResource ? OutputResource->GetRenderTargetTexture() : FTextureRHIRef();

	if (!SceneColorTextureRHI.IsValid() || !OutputTextureRHI.IsValid())
	{
		UE_LOG(LogShaderExecutor, Error, TEXT("Cannot run colour extract pass: render target resources are invalid."));
		return;
	}

	// Convert the class variables to variables that can be used by the render command lambda
	const FVector3f TargetColourVector = FVector3f(TargetColour);
	const float ColorToleranceValue = Tolerance;
	const FVector3f DeltaWeightsVector = DeltaWeights;
	const bool bDebugOutputValue = bDebugOutput;

	// Add an enqueued render command to add our colour extract render pass immediately
	ENQUEUE_RENDER_COMMAND(RunColourExtractRenderPassNow)(
		[SceneColorTextureRHI, OutputTextureRHI, TargetColourVector, ColorToleranceValue, DeltaWeightsVector, 
			bDebugOutputValue](FRHICommandListImmediate& RHICmdList)
		{
			FRDGBuilder GraphBuilder(RHICmdList);
						
			// Create RDG textures from the RHI textures
			const TRefCountPtr<IPooledRenderTarget> SceneColourPooledRef = CreateRenderTarget(SceneColorTextureRHI, TEXT("ShaderExecutor.SceneColor"));
			const TRefCountPtr<IPooledRenderTarget> OutputPooledRef = CreateRenderTarget(OutputTextureRHI, TEXT("ShaderExecutor.Output"));
			
			const FRDGTextureRef SceneColorTexture = GraphBuilder.RegisterExternalTexture(SceneColourPooledRef);
			const FRDGTextureRef OutputTexture = GraphBuilder.RegisterExternalTexture(OutputPooledRef);

			const FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
			if (!ensure(SceneColorTexture && OutputTexture && GlobalShaderMap))
			{
				return;
			}
			
			// Set up the parameters
			FColourExtractPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FColourExtractPS::FParameters>();
			PassParameters->TargetColour = TargetColourVector;
			PassParameters->Tolerance = ColorToleranceValue;
			PassParameters->DeltaWeights = DeltaWeightsVector;
			PassParameters->SceneColorTexture = SceneColorTexture;
			PassParameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::ENoAction);
			
			// Set up the permutation vectors
			FColourExtractPS::FPermutationDomain PermutationVector;
			PermutationVector.Set<FColourExtractPS::FDebugOutput>(bDebugOutputValue);
			
			// Render a full screen size pass
			TShaderMapRef<FColourExtractPS> PixelShader(GlobalShaderMap, PermutationVector);
			FPixelShaderUtils::AddFullscreenPass<FColourExtractPS>(
				GraphBuilder,
				GlobalShaderMap,
				RDG_EVENT_NAME("ColourExtractPass"),
				PixelShader,
				PassParameters,
				FIntRect(FIntPoint::ZeroValue, OutputTexture->Desc.Extent));

			// This converts the RDG graph into RHI commands to be sent to the GPU and executed
			GraphBuilder.Execute();
		});

	// Like before we need to flush the render commands so that the render thread has finished executing the colour extract pass
	FlushRenderingCommands();
#else
	UE_LOG(LogShaderExecutor, Warning, TEXT("RunColourExtractRenderPass is editor-only."));
#endif
}

#if WITH_EDITOR
UTextureRenderTarget2D* AShaderExecutor::GetOrCreateColourExtractRenderTarget(const TCHAR* ObjectPath, const FIntPoint& Resolution, 
	ETextureRenderTargetFormat Format)
{
	UTextureRenderTarget2D* RenderTarget = LoadObject<UTextureRenderTarget2D>(nullptr, ObjectPath);
	const bool bNeedsNewAsset = RenderTarget == nullptr;

	// Create a new asset if it doesn't already exist
	if (bNeedsNewAsset)
	{
		const FString ObjectPathString = ObjectPath;
		const FString PackageName = FPackageName::ObjectPathToPackageName(ObjectPathString);
		const FString AssetName = FPackageName::GetLongPackageAssetName(PackageName);
		UPackage* Package = CreatePackage(*PackageName);

		RenderTarget = NewObject<UTextureRenderTarget2D>(Package, *AssetName, RF_Public | RF_Standalone);
		if (!RenderTarget)
		{
			UE_LOG(LogShaderExecutor, Error, TEXT("Failed to create render target asset '%s'."), ObjectPath);
			return nullptr;
		}

		FAssetRegistryModule::AssetCreated(RenderTarget);
		Package->MarkPackageDirty();
	}

	const bool bNeedsResize = RenderTarget->SizeX != Resolution.X || RenderTarget->SizeY != Resolution.Y;
	const bool bNeedsFormat = RenderTarget->RenderTargetFormat != Format;

	// Change the format if needed
	if (bNeedsFormat)
	{
		RenderTarget->RenderTargetFormat = Format;
	}

	// Update the resources as soon as
	if (bNeedsResize || bNeedsFormat || bNeedsNewAsset)
	{
		RenderTarget->ClearColor = FLinearColor::Black;
		RenderTarget->AddressX = TA_Clamp;
		RenderTarget->AddressY = TA_Clamp;
		RenderTarget->bAutoGenerateMips = false;
		RenderTarget->InitAutoFormat(Resolution.X, Resolution.Y);
		RenderTarget->UpdateResourceImmediate(true);
		RenderTarget->MarkPackageDirty();

		// Try and save the asset
		if (!SaveColourExtractRenderTarget(RenderTarget))
		{
			return nullptr;
		}
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
