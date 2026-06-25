// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ShaderExecutor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UE5_TUT_9_IMMEDIATE_SHADER_ShaderExecutor_generated_h
#error "ShaderExecutor.generated.h already included, missing '#pragma once' in ShaderExecutor.h"
#endif
#define UE5_TUT_9_IMMEDIATE_SHADER_ShaderExecutor_generated_h

#define FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execRunColourExtractRenderPass);


#define FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAShaderExecutor(); \
	friend struct Z_Construct_UClass_AShaderExecutor_Statics; \
public: \
	DECLARE_CLASS(AShaderExecutor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/UE5_Tut_9_Immediate_Shader"), NO_API) \
	DECLARE_SERIALIZER(AShaderExecutor)


#define FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_14_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	AShaderExecutor(AShaderExecutor&&); \
	AShaderExecutor(const AShaderExecutor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AShaderExecutor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AShaderExecutor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AShaderExecutor) \
	NO_API virtual ~AShaderExecutor();


#define FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_11_PROLOG
#define FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_14_INCLASS_NO_PURE_DECLS \
	FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UE5_TUT_9_IMMEDIATE_SHADER_API UClass* StaticClass<class AShaderExecutor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Plugins_tut_009_immediate_shader_Source_UE5_Tut_9_Immediate_Shader_Public_ShaderExecutor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
