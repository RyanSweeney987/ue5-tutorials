// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleActor.h"


void AExampleActor::DynamicDelegateFunc(FSingleBindingDynamicDelegate InMyDynamicDelegate)
{
	UE_LOG(LogTemp, Warning, TEXT("Call DynamicDelegate CPP"));
	
	// Executes the bound event if it has been bound
	InMyDynamicDelegate.ExecuteIfBound();
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
	
	MyDynamicDelegateReturnDelegate = InDynamicDelegateWithReturnValue;
}

int32 AExampleActor::ExecuteStoredDynamicDelegateReturnFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("Execute Stored DynamicDelegateReturnFunc CPP"));
	
	if (MyDynamicDelegateReturnDelegate.IsBound())
	{
		return MyDynamicDelegateReturnDelegate.Execute();
	}

	return 4;
}

void AExampleActor::ExecuteCPPBindings() const
{
	UE_LOG(LogTemp, Warning, TEXT("ExecuteCPPBindings CPP"));
	
	// Single binding only delegates
	ExampleDelegate.ExecuteIfBound();
	
	// Multicast delegates
	SimpleMulticastDelegate.Broadcast();
	
	// For use with return values
	if(MyDelegateWithReturnValue.IsBound())
	{
		int32 ReturnValue = MyDelegateWithReturnValue.Execute();
	}
	
	// Call the TFunction
	if(MyFunction)
	{
		MyFunction();
	}
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
