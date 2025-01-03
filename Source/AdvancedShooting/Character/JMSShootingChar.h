// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JMSCharBase.h"
#include "JMSShootingChar.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDSHOOTING_API AJMSShootingChar : public AJMSCharBase
{
	GENERATED_BODY()

public:
	AJMSShootingChar();

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_SwitchWeapon;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_AIM;

protected:
	UFUNCTION()
	void SwitchWeapon(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Aim(const FInputActionValue& InputActionValue);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	
};
