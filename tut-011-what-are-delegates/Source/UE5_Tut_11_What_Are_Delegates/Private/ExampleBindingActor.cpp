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
	BindMulticastDelegates(ExampleActor);
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
	InActor->SingleBindingDelegate.BindLambda([]()
	{
		UE_LOG(LogTemp, Warning, TEXT("ExampleDelegate CPP"));
	});
	
	// Requires UObject (this)
	InActor->SingleBindingDelegate.BindWeakLambda(this, [this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("BindWeakLambda CPP"));
	});
	InActor->SingleBindingDelegate.BindUObject(this, &AExampleBindingActor::UObjectBinding);
	InActor->SingleBindingDelegate.BindUFunction(this, FName("UFunctionBinding"));
	
	if(!RawBindingActor)
	{
		return;
	} 
	
	// Standard C++ object ptr
	InActor->SingleBindingDelegate.BindRaw(RawBindingActor, &FRawClass::RawFunctionBinding);
	
	if(!SharedBindingActor)
	{
		return;
	}
	// Get the underlying ptr
	InActor->SingleBindingDelegate.BindRaw(SharedBindingActor.Get(), &FSharedClass::SharedFunctionBinding);
	// Shared ref - can get from TSharedPtr
	InActor->SingleBindingDelegate.BindSP(SharedBindingActor.ToSharedRef(), &FSharedClass::SharedFunctionBinding);
	
	// Static function binding
	InActor->SingleBindingDelegate.BindStatic(AExampleBindingActor::StaticFunctionBinding);
}

void AExampleBindingActor::BindMulticastDelegates(AExampleActor* InActor)
{
	// All of these bindings will get called because of it being a multicast delegate
	
	// General lambda
	InActor->MultipleBindingsDelegate.AddLambda([]()
	{
		UE_LOG(LogTemp, Warning, TEXT("ExampleDelegate CPP"));
	});
	
	// Requires UObject (this)
	InActor->MultipleBindingsDelegate.AddWeakLambda(this, [this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("BindWeakLambda CPP"));
	});
	InActor->MultipleBindingsDelegate.AddUObject(this, &AExampleBindingActor::UObjectBinding);
	InActor->MultipleBindingsDelegate.AddUFunction(this, FName("UFunctionBinding"));

	if(!RawBindingActor)
	{
		return;
	} 
	
	// Standard C++ object ptr
	InActor->MultipleBindingsDelegate.AddRaw(RawBindingActor, &FRawClass::RawFunctionBinding);
	
	if(!SharedBindingActor)
	{
		return;
	}
	// Get the underlying ptr
	InActor->MultipleBindingsDelegate.AddRaw(SharedBindingActor.Get(), &FSharedClass::SharedFunctionBinding);
	// Shared ref - can get from TSharedPtr
	InActor->MultipleBindingsDelegate.AddSP(SharedBindingActor.ToSharedRef(), &FSharedClass::SharedFunctionBinding);
	
	// Static function binding
	InActor->MultipleBindingsDelegate.AddStatic(AExampleBindingActor::StaticFunctionBinding);
}

void AExampleBindingActor::BindDynamicDelegates(AExampleActor* InActor)
{
	// Can only bind UFUNCTIONS
	InActor->SingleBindingDynamicDelegate.BindUFunction(this, FName("UFunctionBinding"));
	InActor->SingleBindingDynamicDelegate.BindDynamic(this, &AExampleBindingActor::UFunctionBinding);
		
	// Have to use a macro to bind in CPP
	// Also doesn't like the target function being const
	InActor->ReadWriteMulticastDelegate.AddDynamic(this, &AExampleBindingActor::UFunctionBinding);
	InActor->ReadWriteMulticastDelegate.AddUniqueDynamic(this, &AExampleBindingActor::UFunctionBinding);
	
	// Like a normal multicast delegate
	InActor->MultipleBindingDynamicSparseDelegate.AddDynamic(this, &AExampleBindingActor::USparseFunctionBinding);
	InActor->MultipleBindingDynamicSparseDelegate.AddUniqueDynamic(this, &AExampleBindingActor::USparseFunctionBinding);
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

void AExampleBindingActor::UFunctionBinding()
{
	UE_LOG(LogTemp, Warning, TEXT("UFunctionBinding CPP"));
}

void AExampleBindingActor::USparseFunctionBinding(AExampleActor* ExampleActor)
{
	
}

void AExampleBindingActor::StaticFunctionBinding()
{
	UE_LOG(LogTemp, Warning, TEXT("StaticFunctionBinding CPP"));
}

