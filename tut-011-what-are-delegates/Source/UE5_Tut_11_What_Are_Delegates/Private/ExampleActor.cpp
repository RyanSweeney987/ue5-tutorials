// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleActor.h"




void AExampleActor::DynamicDelegateFunc(FSingleBindingDynamicDelegate InMyDynamicDelegate)
{
	UE_LOG(LogTemp, Warning, TEXT("Call DynamicDelegate CPP"));
	
	// Executes the bound event if it has been bound
	InMyDynamicDelegate.ExecuteIfBound();
}

void AExampleActor::Dynamic2DelegateFunc(FSingleBindingDynamicDelegate MyDynamicDelegate,
	FSingleBindingDynamicDelegate AnotherDynamicDelegate)
{
	UE_LOG(LogTemp, Warning, TEXT("Call Dynamic2DelegateFunc CPP"));
	
	if(MyDynamicDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("MyDynamicDelegate is bound"));
		MyDynamicDelegate.Execute();
	}
	
	if(AnotherDynamicDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("AnotherDynamicDelegate is bound"));
		AnotherDynamicDelegate.Execute();
	}
}

void AExampleActor::SetDynamicDelegateFunc(FSingleBindingDynamicDelegate MyDynamicDelegate)
{
	SingleBindingDynamicDelegate = MyDynamicDelegate;
}

int32 AExampleActor::DynamicDelegateReturnFunc(FSingleBindingDynamicWithReturnValue InDynamicDelegateWithReturnValue)
{	
	UE_LOG(LogTemp, Warning, TEXT("Call DynamicDelegateReturnFunc CPP"));
	
	// Execute the bound event if it has been bound
	// This type has no "ExecuteIfBound" function so check separately
	if(InDynamicDelegateWithReturnValue.IsBound())
	{
		return InDynamicDelegateWithReturnValue.Execute();
	}
	
	return 2;
}

void AExampleActor::SetDynamicDelegateReturnFunc(FSingleBindingDynamicWithReturnValue InDynamicDelegateWithReturnValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Set DynamicDelegateReturnFunc CPP"));
	
	SingleBindingDynamicReturnDelegate = InDynamicDelegateWithReturnValue;
}

void AExampleActor::SetDynamicOneParamDelegateReturnFunc(
	FSingleBindingOneParamDynamicWithReturnValue InSingleBindingOneParamDynamicReturnDelegate)
{
	UE_LOG(LogTemp, Warning, TEXT("Set DynamicOneParamDelegateReturnFunc CPP"));
	
	SingleBindingOneParamDynamicReturnDelegate = InSingleBindingOneParamDynamicReturnDelegate;
}

void AExampleActor::BPNativeEvent_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Call to BPNativeEvent_Implementation"));
}

void AExampleActor::BPNativeValueEvent_Implementation(int32 Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Call to BPNativeValueEvent_Implementation with value: %d"), Value);
}

int32 AExampleActor::BPReturnNativeEvent_Implementation()
{
	return 213123123;
}

void AExampleActor::ExecuteStoredDynamicDelegateFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("Execute Stored DynamicDelegateFunc CPP"));
	
	if (SingleBindingDynamicDelegate.IsBound())
	{
		SingleBindingDynamicDelegate.Execute();
	}
}

int32 AExampleActor::ExecuteStoredDynamicDelegateReturnFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("Execute Stored DynamicDelegateReturnFunc CPP"));
	
	if (SingleBindingDynamicReturnDelegate.IsBound())
	{
		return SingleBindingDynamicReturnDelegate.Execute();
	}
	
	return 4;
}

void AExampleActor::ExecuteCPPBindings()
{
	UE_LOG(LogTemp, Warning, TEXT("ExecuteCPPBindings CPP"));
	
	// Single binding only delegates
	SingleBindingDelegate.ExecuteIfBound();
	
	// Multicast delegates
	MultipleBindingsDelegate.Broadcast();
	
	// For use with return values
	if(SingleBindingWithReturnValue.IsBound())
	{
		int32 ReturnValue = SingleBindingWithReturnValue.Execute();
	}
	
	// Call the TFunction
	if(MyFunction)
	{
		MyFunction();
	}
	
	BPImplementableEvent();
	int32 Result = BPReturnImplementableEvent();
	
	// Call the original name, not the _Implementation version
	BPNativeEvent();
}

// Sets default values
AExampleActor::AExampleActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AExampleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExampleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
