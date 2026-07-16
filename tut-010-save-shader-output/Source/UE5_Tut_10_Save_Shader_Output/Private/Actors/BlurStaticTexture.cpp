#include "Actors/BlurStaticTexture.h"

#include "Engine/Texture2D.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "RHICommandList.h"
#include "Data/BlurRequests.h"
#include "Subsystems/SaveShaderOutputSubsystem.h"
#include "UObject/Package.h"

ABlurStaticTexture::ABlurStaticTexture()
{
	PrimaryActorTick.bCanEverTick = false;
}

/**
 * Offloads the work to a scene view extension
 */
void ABlurStaticTexture::RunBlur() const
{
	if(!SourceTexture)
	{	
		UE_LOG(LogTemp, Warning, TEXT("BlurStaticTexture: SourceTexture is not set."));
		return;
	}
	
	if(USaveShaderOutputSubsystem* Subsystem = USaveShaderOutputSubsystem::Get())
	{
		Subsystem->QueueBlurRequest(FBlurRequestData(SourceTexture, BlurRadius, bDownloadImmediately, OutputAssetPath, OutputAssetNamePrefix));
	}
}
