// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UE5_Tut_10_Static_Texture_In_Shader/Public/Actors/BlurStaticTexture.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBlurStaticTexture() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AActor();
ENGINE_API UClass* Z_Construct_UClass_UTexture_NoRegister();
UE5_TUT_10_STATIC_TEXTURE_IN_SHADER_API UClass* Z_Construct_UClass_ABlurStaticTexture();
UE5_TUT_10_STATIC_TEXTURE_IN_SHADER_API UClass* Z_Construct_UClass_ABlurStaticTexture_NoRegister();
UPackage* Z_Construct_UPackage__Script_UE5_Tut_10_Static_Texture_In_Shader();
// End Cross Module References

// Begin Class ABlurStaticTexture Function RunBlurAndSaveTexture
struct Z_Construct_UFunction_ABlurStaticTexture_RunBlurAndSaveTexture_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "Blur" },
		{ "DisplayName", "Run Blur and Save Texture" },
		{ "ModuleRelativePath", "Public/Actors/BlurStaticTexture.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABlurStaticTexture_RunBlurAndSaveTexture_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABlurStaticTexture, nullptr, "RunBlurAndSaveTexture", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ABlurStaticTexture_RunBlurAndSaveTexture_Statics::Function_MetaDataParams), Z_Construct_UFunction_ABlurStaticTexture_RunBlurAndSaveTexture_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_ABlurStaticTexture_RunBlurAndSaveTexture()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABlurStaticTexture_RunBlurAndSaveTexture_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ABlurStaticTexture::execRunBlurAndSaveTexture)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RunBlurAndSaveTexture();
	P_NATIVE_END;
}
// End Class ABlurStaticTexture Function RunBlurAndSaveTexture

// Begin Class ABlurStaticTexture
void ABlurStaticTexture::StaticRegisterNativesABlurStaticTexture()
{
	UClass* Class = ABlurStaticTexture::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "RunBlurAndSaveTexture", &ABlurStaticTexture::execRunBlurAndSaveTexture },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABlurStaticTexture);
UClass* Z_Construct_UClass_ABlurStaticTexture_NoRegister()
{
	return ABlurStaticTexture::StaticClass();
}
struct Z_Construct_UClass_ABlurStaticTexture_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Actors/BlurStaticTexture.h" },
		{ "ModuleRelativePath", "Public/Actors/BlurStaticTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SourceTexture_MetaData[] = {
		{ "Category", "Blur" },
		{ "ModuleRelativePath", "Public/Actors/BlurStaticTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OutputAssetPath_MetaData[] = {
		{ "Category", "Blur" },
		{ "ModuleRelativePath", "Public/Actors/BlurStaticTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OutputAssetNamePrefix_MetaData[] = {
		{ "Category", "Blur" },
		{ "ModuleRelativePath", "Public/Actors/BlurStaticTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bDownloadImmediately_MetaData[] = {
		{ "Category", "Blur" },
		{ "ModuleRelativePath", "Public/Actors/BlurStaticTexture.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SourceTexture;
	static const UECodeGen_Private::FStrPropertyParams NewProp_OutputAssetPath;
	static const UECodeGen_Private::FStrPropertyParams NewProp_OutputAssetNamePrefix;
	static void NewProp_bDownloadImmediately_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bDownloadImmediately;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ABlurStaticTexture_RunBlurAndSaveTexture, "RunBlurAndSaveTexture" }, // 867111809
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABlurStaticTexture>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_SourceTexture = { "SourceTexture", nullptr, (EPropertyFlags)0x0114000000000001, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABlurStaticTexture, SourceTexture), Z_Construct_UClass_UTexture_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SourceTexture_MetaData), NewProp_SourceTexture_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_OutputAssetPath = { "OutputAssetPath", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABlurStaticTexture, OutputAssetPath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OutputAssetPath_MetaData), NewProp_OutputAssetPath_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_OutputAssetNamePrefix = { "OutputAssetNamePrefix", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABlurStaticTexture, OutputAssetNamePrefix), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OutputAssetNamePrefix_MetaData), NewProp_OutputAssetNamePrefix_MetaData) };
void Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_bDownloadImmediately_SetBit(void* Obj)
{
	((ABlurStaticTexture*)Obj)->bDownloadImmediately = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_bDownloadImmediately = { "bDownloadImmediately", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ABlurStaticTexture), &Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_bDownloadImmediately_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDownloadImmediately_MetaData), NewProp_bDownloadImmediately_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ABlurStaticTexture_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_SourceTexture,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_OutputAssetPath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_OutputAssetNamePrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABlurStaticTexture_Statics::NewProp_bDownloadImmediately,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABlurStaticTexture_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ABlurStaticTexture_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_UE5_Tut_10_Static_Texture_In_Shader,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABlurStaticTexture_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ABlurStaticTexture_Statics::ClassParams = {
	&ABlurStaticTexture::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ABlurStaticTexture_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ABlurStaticTexture_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ABlurStaticTexture_Statics::Class_MetaDataParams), Z_Construct_UClass_ABlurStaticTexture_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ABlurStaticTexture()
{
	if (!Z_Registration_Info_UClass_ABlurStaticTexture.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABlurStaticTexture.OuterSingleton, Z_Construct_UClass_ABlurStaticTexture_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ABlurStaticTexture.OuterSingleton;
}
template<> UE5_TUT_10_STATIC_TEXTURE_IN_SHADER_API UClass* StaticClass<ABlurStaticTexture>()
{
	return ABlurStaticTexture::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ABlurStaticTexture);
ABlurStaticTexture::~ABlurStaticTexture() {}
// End Class ABlurStaticTexture

// Begin Registration
struct Z_CompiledInDeferFile_FID_Plugins_tut_010_static_texture_in_shader_Source_UE5_Tut_10_Static_Texture_In_Shader_Public_Actors_BlurStaticTexture_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ABlurStaticTexture, ABlurStaticTexture::StaticClass, TEXT("ABlurStaticTexture"), &Z_Registration_Info_UClass_ABlurStaticTexture, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABlurStaticTexture), 749460568U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Plugins_tut_010_static_texture_in_shader_Source_UE5_Tut_10_Static_Texture_In_Shader_Public_Actors_BlurStaticTexture_h_1594113017(TEXT("/Script/UE5_Tut_10_Static_Texture_In_Shader"),
	Z_CompiledInDeferFile_FID_Plugins_tut_010_static_texture_in_shader_Source_UE5_Tut_10_Static_Texture_In_Shader_Public_Actors_BlurStaticTexture_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Plugins_tut_010_static_texture_in_shader_Source_UE5_Tut_10_Static_Texture_In_Shader_Public_Actors_BlurStaticTexture_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
