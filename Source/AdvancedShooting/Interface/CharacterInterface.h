// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEDSHOOTING_API ICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void IncreaseCharacterHealth(int32 Amount) = 0;
	virtual void IncreaseCharacterShield(int32 Amount) = 0;
	virtual void IncreaseCharacterPistolClip(int32 Amount) = 0;
	virtual void IncreaseCharacterRifleClip(int32 Amount) = 0;
};
