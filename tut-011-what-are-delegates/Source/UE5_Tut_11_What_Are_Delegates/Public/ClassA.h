// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_DELEGATE(FTutorialDelegate);

class FClassA
{
	FTutorialDelegate OnNuclearApocalypseDelegate;
	
public:
	FTutorialDelegate& GetOnNuclearApocalypse()
	{
		return OnNuclearApocalypseDelegate;
	}
	
private:
	void NukesLaunched() const
	{
		OnNuclearApocalypseDelegate.ExecuteIfBound();
	}
};

class FClassB
{
public:
	void SubscribeToEndOfTheWorld(FClassA& InClass)
	{
		InClass.GetOnNuclearApocalypse().BindRaw(this, &FClassB::OnNuclearApocalypse);
	}
	
private:
	void OnNuclearApocalypse()
	{
		// Take shelter or (✖╭╮✖)
	}
};