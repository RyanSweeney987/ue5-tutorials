// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExampleBindingActor.generated.h"

class AExampleActor;

class FRawClass
{
public:
	void RawFunctionBinding() const;
};

class FSharedClass
{
public:
	void SharedFunctionBinding() const;
};

UCLASS()
class UE5_TUT_11_WHAT_ARE_DELEGATES_API AExampleBindingActor : public AActor
{
	GENERATED_BODY()

	FRawClass* RawBindingActor;
	TSharedPtr<FSharedClass> SharedBindingActor;
	
public:
	// Sets default values for this actor's properties
	AExampleBindingActor();
	virtual ~AExampleBindingActor() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void BindDelegates(AExampleActor* InActor);
	void BindDynamicDelegates(AExampleActor* InActor);
	void BindOther(AExampleActor* InActor);
	
	void RawFunctionBinding() const;
	void UObjectBinding() const;
	
	UFUNCTION(BlueprintCallable)
	void UFunctionBinding() const;
	
	static void StaticFunctionBinding();
};
