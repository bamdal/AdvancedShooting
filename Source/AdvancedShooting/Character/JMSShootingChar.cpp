// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSShootingChar.h"

#include "EnhancedInputComponent.h"


AJMSShootingChar::AJMSShootingChar()
{
}


void AJMSShootingChar::SwitchWeapon(const FInputActionValue& InputActionValue)
{
	float Result = InputActionValue.Get<float>();
	//GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%f"),Result));
}

void AJMSShootingChar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_SwitchWeapon,ETriggerEvent::Triggered,this,&AJMSShootingChar::SwitchWeapon);
		EnhancedInputComponent->BindAction(IA_SwitchWeapon,ETriggerEvent::Started,this,&AJMSShootingChar::SwitchWeapon);
	}
}
