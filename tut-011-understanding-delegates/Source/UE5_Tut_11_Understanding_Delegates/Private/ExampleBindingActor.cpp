// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleBindingActor.h"

#include "EngineUtils.h"
#include "ExampleActor.h"


void FRawClass::RawFunctionBinding() const
{
	UE_LOG(LogTemp, Warning, TEXT("RawFunctionBinding CPP"));
}

void FSharedClass::SharedFunctionBinding() const
{
	UE_LOG(LogTemp, Warning, TEXT("SharedFunctionBinding CPP"));
}

// Sets default values
AExampleBindingActor::AExampleBindingActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

AExampleBindingActor::~AExampleBindingActor()
{
	// Clear our memory
	delete RawBindingActor;
	RawBindingActor = nullptr;
	
	SharedBindingActor.Reset();
}

// Called when the game starts or when spawned
void AExampleBindingActor::BeginPlay()
{
	Super::BeginPlay();
	
	const UWorld* CurrentWorld = GetWorld();
	
	// Make sure the world exists
	if(!CurrentWorld)
	{
		return;
	}
	
	AExampleActor* ExampleActor = nullptr;
	// Iterate through all the actors of type in the world
	for(TActorIterator<AExampleActor> It(CurrentWorld); It; ++It)
	{
		if(const AActor* Actor = *It; IsValid(Actor) && 
			!Actor->HasAnyFlags(RF_Transient | RF_ClassDefaultObject) && 
			!Actor->IsTemplate() && 
			!Actor->IsActorBeingDestroyed())
		{
			ExampleActor = *It;
			break;
		}
	}
	
	// Make sure we found an actor
	if(!ExampleActor)
	{
		return;
	}
	
	// Create these types for the examples
	RawBindingActor = new FRawClass();
	SharedBindingActor = MakeShared<FSharedClass>();
	
	// Initiate the binding ritual
	BindDelegates(ExampleActor);
	BindDynamicDelegates(ExampleActor);
}

// Called every frame
void AExampleBindingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExampleBindingActor::BindDelegates(AExampleActor* InActor)
{
	// Each binding delegate on a non-multicast delegate will replace the existing binding.
	// The shared ref binding should be what runs if it gets executed/broadcasted
	
	// General lambda
	InActor->ExampleDelegate.BindLambda([]()
	{
		UE_LOG(LogTemp, Warning, TEXT("ExampleDelegate CPP"));
	});
	
	// Requires UObject (this)
	InActor->ExampleDelegate.BindWeakLambda(this, [this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("BindWeakLambda CPP"));
	});
	InActor->ExampleDelegate.BindUObject(this, &AExampleBindingActor::UObjectBinding);
	InActor->ExampleDelegate.BindUFunction(this, FName("UFunctionBinding"));
	
	if(!RawBindingActor)
	{
		return;
	} 
	
	// Standard C++ object ptr
	InActor->ExampleDelegate.BindRaw(RawBindingActor, &FRawClass::RawFunctionBinding);
	
	if(!SharedBindingActor)
	{
		return;
	}
	// Get the underlying ptr
	InActor->ExampleDelegate.BindRaw(SharedBindingActor.Get(), &FSharedClass::SharedFunctionBinding);
	// Shared ref - can get from TSharedPtr
	InActor->ExampleDelegate.BindSP(SharedBindingActor.ToSharedRef(), &FSharedClass::SharedFunctionBinding);
	
	// Static function binding
	InActor->ExampleDelegate.BindStatic(AExampleBindingActor::StaticFunctionBinding);
}

void AExampleBindingActor::BindDynamicDelegates(AExampleActor* InActor)
{
	// Dynamic delegates bind like native only delegates
	
	// All of these bindings will get called because of it being a multicast delegate
	// Native multicast delegates are bound the same way
	
	// General lambda
	InActor->SimpleMulticastDelegate.AddLambda([]()
	{
		UE_LOG(LogTemp, Warning, TEXT("ExampleDelegate CPP"));
	});
	
	// Requires UObject (this)
	InActor->SimpleMulticastDelegate.AddWeakLambda(this, [this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("BindWeakLambda CPP"));
	});
	InActor->SimpleMulticastDelegate.AddUObject(this, &AExampleBindingActor::UObjectBinding);
	InActor->SimpleMulticastDelegate.AddUFunction(this, FName("UFunctionBinding"));

	if(!RawBindingActor)
	{
		return;
	} 
	
	// Standard C++ object ptr
	InActor->SimpleMulticastDelegate.AddRaw(RawBindingActor, &FRawClass::RawFunctionBinding);
	
	if(!SharedBindingActor)
	{
		return;
	}
	// Get the underlying ptr
	InActor->SimpleMulticastDelegate.AddRaw(SharedBindingActor.Get(), &FSharedClass::SharedFunctionBinding);
	// Shared ref - can get from TSharedPtr
	InActor->SimpleMulticastDelegate.AddSP(SharedBindingActor.ToSharedRef(), &FSharedClass::SharedFunctionBinding);
	
	// Static function binding
	InActor->SimpleMulticastDelegate.AddStatic(AExampleBindingActor::StaticFunctionBinding);
}

void AExampleBindingActor::BindOther(AExampleActor* InActor)
{
	// We can bind a lambda to the TFunction
	InActor->MyFunction = [this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("MyFunction CPP"));
		
		// Call class local function here
		RawFunctionBinding();
	};
	
	// Or bind to a static function
	InActor->MyFunction = AExampleBindingActor::StaticFunctionBinding;
}

void AExampleBindingActor::RawFunctionBinding() const
{
	UE_LOG(LogTemp, Warning, TEXT("RawFunctionBinding CPP"));
}

void AExampleBindingActor::UObjectBinding() const
{
	UE_LOG(LogTemp, Warning, TEXT("UObjectBinding CPP"));
}

void AExampleBindingActor::UFunctionBinding() const
{
	UE_LOG(LogTemp, Warning, TEXT("UFunctionBinding CPP"));
}

void AExampleBindingActor::StaticFunctionBinding()
{
	UE_LOG(LogTemp, Warning, TEXT("StaticFunctionBinding CPP"));
}

