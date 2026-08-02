// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include "ExampleActor.generated.h"

UCLASS()
class UE5_TUT_11_UNDERSTANDING_DELEGATES_API AExampleActor : public AActor
{
	GENERATED_BODY()

	// Delegates are found in Delegate.h
	
	// TDelegate and TMulticastDelegate are the underlying classes used in the macros
	// Macros use typedef
	// 
	// TDelegate<void()> BaseDelegate;
	// TMulticastDelegate<void()> BaseMulticastDelegate;
	
	// Native - faster
	// Single binding
	DECLARE_DELEGATE(FMyDelegate)
	// DECLARE_DELEGATE_OneParam(FMyDelegate, FParamType);
	// Multiple bindings
	DECLARE_MULTICAST_DELEGATE(FMyMulticastDelegate)
	DECLARE_TS_MULTICAST_DELEGATE(FMyThreadSafeMulticastDelegate)
	// Single binding		- return value 
	DECLARE_DELEGATE_RetVal(int32, FMyDelegateWithReturnValue)
	
	// Param must be include in the bound function input parameters
	// DECLARE_DELEGATE_OneParam(FMyDelegateWithOneParam, int32);
	
	// Return type - delegate name type - input parameter
	// Bound functions must have the appropriate return type and input parameters
	// DECLARE_DELEGATE_RetVal_OneParam(int32, FMyDelegateWithReturnValueAndOneParam, int32);
	
	// --------------------------------------------------------
		
	// TBaseDynamicDelegate and TBaseDynamicMulticastDelegate are the base for blueprint exposed dynamic delegates
	// Macros extend these types
	// 
	// TBaseDynamicDelegate<FNotThreadSafeDelegateMode, void()> BaseDynamicDelegate;
	// TBaseDynamicMulticastDelegate<FNotThreadSafeDelegateMode, void()> BaseDynamicMulticastDelegate;
	
	// Serialisation/blueprint aware
	// Single binding
	DECLARE_DYNAMIC_DELEGATE(FMyDynamicDelegate);
	// Multiple bindings
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyDynamicMulticastDelegate);
	// Single binding		- return value 
	DECLARE_DYNAMIC_DELEGATE_RetVal(int32, FMyDynamicDelegateWithReturnValue);
	
	// Exists but don't use - consider as deprecated - uses multicast delegate under the hood
	DECLARE_EVENT(AExampleActor, FMyEvent)

public:
	// Can save a function for a callback
	TFunction<void()> MyFunction;
	
	// DECLARE_... macros create types
	// You need to create a global/class variable of it to use it
	FMyDelegate ExampleDelegate;
	FMyMulticastDelegate MyMulticastDelegate;
	FMyThreadSafeMulticastDelegate MyThreadSafeMulticastDelegate;
	FMyDelegateWithReturnValue MyDelegateWithReturnValue;
	
	// These types are already declared with DECLARE_... macros
	FSimpleDelegate SimpleDelegate;
	FSimpleMulticastDelegate SimpleMulticastDelegate;
	FTSSimpleMulticastDelegate SimpleThreadSafeMulticastDelegate;
	
	// Store an event callback, set with the SetDynamicDelegateReturnFunc
	// Can't access this parameter in the BP
	UPROPERTY(BlueprintReadWrite, Category="Delegates Params CPP")
	FMyDynamicDelegateWithReturnValue MyDynamicDelegateReturnDelegate;
	
public:
	// Can call in BP, runs the function and triggers the bound event as a kind of callback if one is bound
	UFUNCTION(BlueprintCallable, Category="Delegates CPP")
	void DynamicDelegateFunc(FMyDynamicDelegate& MyDynamicDelegate);
	
	// Normal event binding, shows up like the other events such as BeginPlay in BP
	UPROPERTY(BlueprintAssignable, Category="Delegates CPP")
	FMyDynamicMulticastDelegate MyDynamicMulticastDelegate;
	
	// Same as DynamicDelegateFunc but includes a return value
	UFUNCTION(BlueprintCallable, Category="Delegates CPP")
	int32 DynamicDelegateReturnFunc(FMyDynamicDelegateWithReturnValue& InDynamicDelegateWithReturnValue);

	// Set a dynamic delegate to be executed later
	UFUNCTION(BlueprintCallable, Category="Delegates CPP")
	void SetDynamicDelegateReturnFunc(FMyDynamicDelegateWithReturnValue& InDynamicDelegateWithReturnValue);

	// Trigger the set dynamic delegate
	UFUNCTION(BlueprintCallable, Category="Delegates CPP")
	int32 ExecuteStoredDynamicDelegateReturnFunc();
	
	// Execute the bound CPP bindings from the BP/Editor
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Delegates CPP")
	void ExecuteCPPBindings() const;
public:
	// Sets default values for this actor's properties
	AExampleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
