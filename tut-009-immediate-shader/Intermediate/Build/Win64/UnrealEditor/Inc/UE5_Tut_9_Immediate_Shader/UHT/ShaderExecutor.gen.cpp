// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UE5_Tut_9_Immediate_Shader/Public/ShaderExecutor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeShaderExecutor() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FLinearColor();
ENGINE_API UClass* Z_Construct_UClass_AActor();
ENGINE_API UClass* Z_Construct_UClass_UTextureRenderTarget2D_NoRegister();
UE5_TUT_9_IMMEDIATE_SHADER_API UClass* Z_Construct_UClass_AShaderExecutor();
UE5_TUT_9_IMMEDIATE_SHADER_API UClass* Z_Construct_UClass_AShaderExecutor_NoRegister();
UPackage* Z_Construct_UPackage__Script_UE5_Tut_9_Immediate_Shader();
// End Cross Module References

// Begin Class AShaderExecutor Function RunColourExtractRenderPass
struct Z_Construct_UFunction_AShaderExecutor_RunColourExtractRenderPass_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "Shader Executor" },
		{ "ModuleRelativePath", "Public/ShaderExecutor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AShaderExecutor_RunColourExtractRenderPass_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AShaderExecutor, nullptr, "RunColourExtractRenderPass", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AShaderExecutor_RunColourExtractRenderPass_Statics::Function_MetaDataParams), Z_Construct_UFunction_AShaderExecutor_RunColourExtractRenderPass_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_AShaderExecutor_RunColourExtractRenderPass()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AShaderExecutor_RunColourExtractRenderPass_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AShaderExecutor::execRunColourExtractRenderPass)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RunColourExtractRenderPass();
	P_NATIVE_END;
}
// End Class AShaderExecutor Function RunColourExtractRenderPass

// Begin Class AShaderExecutor
void AShaderExecutor::StaticRegisterNativesAShaderExecutor()
{
	UClass* Class = AShaderExecutor::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "RunColourExtractRenderPass", &AShaderExecutor::execRunColourExtractRenderPass },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AShaderExecutor);
UClass* Z_Construct_UClass_AShaderExecutor_NoRegister()
{
	return AShaderExecutor::StaticClass();
}
struct Z_Construct_UClass_AShaderExecutor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "ShaderExecutor.h" },
		{ "ModuleRelativePath", "Public/ShaderExecutor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TargetColour_MetaData[] = {
		{ "Category", "Shader Executor" },
		{ "ModuleRelativePath", "Public/ShaderExecutor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseUnlitSceneColour_MetaData[] = {
		{ "Category", "Shader Executor" },
		{ "ModuleRelativePath", "Public/ShaderExecutor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ColourExtractRenderTarget_MetaData[] = {
		{ "Category", "Shader Executor" },
		{ "ModuleRelativePath", "Public/ShaderExecutor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_TargetColour;
	static void NewProp_bUseUnlitSceneColour_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseUnlitSceneColour;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ColourExtractRenderTarget;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_AShaderExecutor_RunColourExtractRenderPass, "RunColourExtractRenderPass" }, // 1477799494
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AShaderExecutor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AShaderExecutor_Statics::NewProp_TargetColour = { "TargetColour", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AShaderExecutor, TargetColour), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TargetColour_MetaData), NewProp_TargetColour_MetaData) };
void Z_Construct_UClass_AShaderExecutor_Statics::NewProp_bUseUnlitSceneColour_SetBit(void* Obj)
{
	((AShaderExecutor*)Obj)->bUseUnlitSceneColour = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AShaderExecutor_Statics::NewProp_bUseUnlitSceneColour = { "bUseUnlitSceneColour", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AShaderExecutor), &Z_Construct_UClass_AShaderExecutor_Statics::NewProp_bUseUnlitSceneColour_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseUnlitSceneColour_MetaData), NewProp_bUseUnlitSceneColour_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AShaderExecutor_Statics::NewProp_ColourExtractRenderTarget = { "ColourExtractRenderTarget", nullptr, (EPropertyFlags)0x0114000000020001, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AShaderExecutor, ColourExtractRenderTarget), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ColourExtractRenderTarget_MetaData), NewProp_ColourExtractRenderTarget_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AShaderExecutor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AShaderExecutor_Statics::NewProp_TargetColour,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AShaderExecutor_Statics::NewProp_bUseUnlitSceneColour,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AShaderExecutor_Statics::NewProp_ColourExtractRenderTarget,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AShaderExecutor_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_AShaderExecutor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_UE5_Tut_9_Immediate_Shader,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AShaderExecutor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AShaderExecutor_Statics::ClassParams = {
	&AShaderExecutor::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_AShaderExecutor_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_AShaderExecutor_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AShaderExecutor_Statics::Class_MetaDataParams), Z_Construct_UClass_AShaderExecutor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AShaderExecutor()
{
	if (!Z_Registration_Info_UClass_AShaderExecutor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AShaderExecutor.OuterSingleton, Z_Construct_UClass_AShaderExecutor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AShaderExecutor.OuterSingleton;
}
template<> UE5_TUT_9_IMMEDIATE_SHADER_API UClass* StaticClass<AShaderExecutor>()
{
	return AShaderExecutor::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AShaderExecutor);
AShaderExecutor::~AShaderExecutor() {}
// End Class AShaderExecutor

// Begin Registration
struct Z_CompiledInDeferFile_FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AShaderExecutor, AShaderExecutor::StaticClass, TEXT("AShaderExecutor"), &Z_Registration_Info_UClass_AShaderExecutor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AShaderExecutor), 4025317276U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_1953564359(TEXT("/Script/UE5_Tut_9_Immediate_Shader"),
	Z_CompiledInDeferFile_FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
