// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// #include "Delegates/Delegate.h"
#include "ExampleActor.generated.h"

UCLASS()
class UE5_TUT_11_WHAT_ARE_DELEGATES_API AExampleActor : public AActor
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
	DECLARE_DELEGATE(FSingleBindingDelegate)
	// DECLARE_DELEGATE_OneParam(FSingleBindingDelegate, int32)
	// DECLARE_DELEGATE_TwoParams(FSingleBindingDelegate, int32, float)

	// Multiple bindings
	DECLARE_MULTICAST_DELEGATE(FMultipleBindingsDelegate)
	// DECLARE_MULTICAST_DELEGATE_OneParam(FMultipleBindingsDelegate, int32)
	// DECLARE_MULTICAST_DELEGATE_TwoParams(FMultipleBindingsDelegate, int32, float)
	DECLARE_TS_MULTICAST_DELEGATE(FThreadSafeMultipleBindingsDelegate)

	// Single binding		- return value 
	DECLARE_DELEGATE_RetVal(int32, FSingleBindingWithReturnValue)
	// DECLARE_DELEGATE_RetVal_OneParam(int32, FSingleBindingWithReturnValue, int32)
	// DECLARE_DELEGATE_RetVal_TwoParams(int32, FSingleBindingWithReturnValue, int32, float)
		
	// --------------------------------------------------------
		
	// TBaseDynamicDelegate and TBaseDynamicMulticastDelegate are the base for blueprint exposed dynamic delegates
	// Macros extend these types
	// 
	// TBaseDynamicDelegate<FNotThreadSafeDelegateMode, void()> BaseDynamicDelegate;
	// TBaseDynamicMulticastDelegate<FNotThreadSafeDelegateMode, void()> BaseDynamicMulticastDelegate;
	
	// Multiple params are the same for dynamic delegates
	
	// Serialisation/blueprint aware dynamic blueprints
	// Single binding
	DECLARE_DYNAMIC_DELEGATE(FSingleBindingDynamicDelegate);
	// DECLARE_DYNAMIC_DELEGATE_OneParam(FSingleBindingDynamicDelegate, ParamName, int32)
	// DECLARE_DYNAMIC_DELEGATE_TwoParams(FSingleBindingDynamicDelegate, ParamName1, int32, ParamName2, float)
	// Multiple bindings
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMultipleBindingsDynamicDelegate);
	// Single binding		- return value 
	DECLARE_DYNAMIC_DELEGATE_RetVal(int32, FSingleBindingDynamicWithReturnValue);
	
	// Exists but don't use - consider as deprecated - uses multicast delegate under the hood
	DECLARE_EVENT(AExampleActor, FMyEvent);
	
	// Unique type, trades lower memory cost in exchange for higher add/remove cost
	// The comments mention higher invoke cost but I'm not entirely sure as it's used in the actor class for things like
	//		OnActorBeginOverlap, OnActorEndOverlap, OnTakeAnyDamage, OnTakePointDamage, OnTakeRadialDamage
	//		which could be triggered very often but rarely bound
	// Invocations are expected to be infrequent
	// Writing it as a non-param version (remove "AExampleActor*, ExampleActor" from the end) builds, but JetBrains Rider doesn't like it
	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FMultipleBindingSparseDynamicSignature, AExampleActor, MultipleBindingDynamicSparseDelegate, AExampleActor*, ExampleActor);
	
	
public:
	// Can save a function for a callback
	TFunction<void()> MyFunction;
	// TFunction<int32(float)> MyInFloatToOutIntFunction;
	
	// DECLARE_... macros create types
	// You need to create a global/class variable of it to use it
	FSingleBindingDelegate SingleBindingDelegate;
	FMultipleBindingsDelegate MultipleBindingsDelegate;
	FThreadSafeMultipleBindingsDelegate ThreadSafeMultipleBindingsDelegate;
	FSingleBindingWithReturnValue SingleBindingWithReturnValue;
	
	// These types are already declared with DECLARE_... macros
	// FSimpleDelegate SimpleDelegate;
	// FSimpleMulticastDelegate SimpleMulticastDelegate;
	// FTSSimpleMulticastDelegate SimpleThreadSafeMulticastDelegate;
	
	// ------------------------------------------------
	//		Multicast Dynamic Delegates
	// ------------------------------------------------
	// Unbind only in BP
	UPROPERTY(BlueprintReadWrite, Category="Delegates CPP|Params|Multicast|RW")
	FMultipleBindingsDynamicDelegate ReadWriteMulticastDelegate;
	// Assign, bind and unbind only in BP
	UPROPERTY(BlueprintAssignable, Category="Delegates CPP|Params|Multicast|A")
	FMultipleBindingsDynamicDelegate AssignableMulticastDelegate;
	// Call and unbind only in BP
	UPROPERTY(BlueprintCallable, Category="Delegates CPP|Params|Multicast|C")
	FMultipleBindingsDynamicDelegate CallableMulticastDelegate;
	// Assign, bind, call and unbind in BP
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegates CPP|Params|Multicast|AC")
	FMultipleBindingsDynamicDelegate AssignCallableMulticastDelegate;
	
	// Example for the sparse dynamic delegate
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegates CPP|Params|Multicast|Sparse")
	FMultipleBindingSparseDynamicSignature MultipleBindingDynamicSparseDelegate;
	
	// ------------------------------------------------
	//		Single Binding Dynamic Delegates
	// ------------------------------------------------
	// Non-BP exposed dynamic delegate are still valid
	// Single binding multicast delegate
	FSingleBindingDynamicDelegate SingleBindingDynamicDelegate;
		
	// Store an event callback, can set with a dedicated setter function
	// Adding the BlueprintReadWrite allows get/set but nothing more
	UPROPERTY(BlueprintReadWrite, Category="Delegates CPP|Params|Single")
	FSingleBindingDynamicWithReturnValue SingleBindingDynamicReturnDelegate;
		
	// ------------------------------------------------
	//		Delegates as Function Parameters
	// ------------------------------------------------
	
	// Can call in BP, runs the function and triggers the bound event as a kind of callback if one is bound
	// Executes the passed delegate if it is bound, but runs the function normally
	UFUNCTION(BlueprintCallable, Category="Delegates CPP|Functions")
	void DynamicDelegateFunc(FSingleBindingDynamicDelegate MyDynamicDelegate);
	
	// Can have multiple delegates passed into the function
	// Executes the passed delegates if it is bound, but runs the function normally
	UFUNCTION(BlueprintCallable, Category="Delegates CPP|Functions")
	void Dynamic2DelegateFunc(FSingleBindingDynamicDelegate MyDynamicDelegate, FSingleBindingDynamicDelegate AnotherDynamicDelegate);
		
	// Sets the dynamic delegate class variable
	UFUNCTION(BlueprintCallable, Category="Delegates CPP|Functions")
	void SetDynamicDelegateFunc(FSingleBindingDynamicDelegate MyDynamicDelegate);
	
	// Same as DynamicDelegateFunc but includes a return value
	// Executes the passed delegate if it is bound, but runs the function normally
	UFUNCTION(BlueprintCallable, Category="Delegates CPP|Functions")
	int32 DynamicDelegateReturnFunc(FSingleBindingDynamicWithReturnValue InDynamicDelegateWithReturnValue);

	// Sets the dynamic delegate class variable so we can execute it later
	// Could also execute the passed delegate here too
	UFUNCTION(BlueprintCallable, Category="Delegates CPP|Functions")
	void SetDynamicDelegateReturnFunc(FSingleBindingDynamicWithReturnValue InDynamicDelegateWithReturnValue);

	// ------------------------------------------------
	//		Other delegate/event related stuff
	// ------------------------------------------------

	// Don't implement it in C++, will cause compiler error, only call in C++
	// Appears as event node
	UFUNCTION(BlueprintImplementableEvent, Category="Delegates CPP|Events")
	void BPImplementableEvent();
	
	// Also don't implement in C++
	// Appears in function tab, override function to implement
	UFUNCTION(BlueprintImplementableEvent, Category="Delegates CPP|Events")
	int32 BPReturnImplementableEvent();
	
	// Like BPImplementableEvent but can be provided default functionality
	UFUNCTION(BlueprintNativeEvent, Category="Delegates CPP|Events")
	void BPNativeEvent();
	
	// ------------------------------------------------
	
	// Trigger the set dynamic delegate
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Delegates CPP|Helper")
	void ExecuteStoredDynamicDelegateFunc();
	
	// Trigger the set dynamic return value delegate
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Delegates CPP|Helper")
	int32 ExecuteStoredDynamicDelegateReturnFunc();
	
	// Execute the bound CPP bindings from the BP/Editor
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Delegates CPP|Helper")
	void ExecuteCPPBindings();
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
