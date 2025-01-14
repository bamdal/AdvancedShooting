// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSShootingChar.h"

#include "EnhancedInputComponent.h"
#include "JMSShootingAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


AJMSShootingChar::AJMSShootingChar()
{
}


void AJMSShootingChar::BeginPlay()
{
	Super::BeginPlay();

	if (ABP_Unarmed)
		GetMesh()->LinkAnimClassLayers(ABP_Unarmed);
	CurrentGate = E_Gate::Jogging;
	UpdateGate(CurrentGate);
}

void AJMSShootingChar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_SwitchWeapon, ETriggerEvent::Triggered, this,
		                                   &AJMSShootingChar::SwitchWeapon);
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Started, this, &AJMSShootingChar::AimStarted);
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Completed, this, &AJMSShootingChar::AimCompleted);
	}
}

void AJMSShootingChar::SwitchWeapon(const FInputActionValue& InputActionValue)
{
	float Result = InputActionValue.Get<float>();
	switch (FMath::TruncToInt32(Result))
	{
	case 1:
		EquippedWeapon = E_Weapon::UnArmed;
		if (ABP_Unarmed)
			GetMesh()->LinkAnimClassLayers(ABP_Unarmed);
		break;
	case 2:
		EquippedWeapon = E_Weapon::Pistol;
		if (ABP_Pistol)
			GetMesh()->LinkAnimClassLayers(ABP_Pistol);
		break;
	case 3:
		EquippedWeapon = E_Weapon::Rifle;
		if (ABP_Rifle)
			GetMesh()->LinkAnimClassLayers(ABP_Rifle);
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("JMSShootingChar EquipGun Null")));
		break;
	}
}

void AJMSShootingChar::AimStarted(const FInputActionValue& InputActionValue)
{
	UpdateGate(E_Gate::Walking);
}

void AJMSShootingChar::AimCompleted(const FInputActionValue& InputActionValue)
{
	UpdateGate(E_Gate::Jogging);
}

void AJMSShootingChar::UpdateGate(E_Gate Gate)
{
	CurrentGate = Gate;

	UJMSShootingAnimInstance* ShootingAnimInstance = Cast<UJMSShootingAnimInstance>(GetMesh()->GetAnimInstance());
	FGateSetting* GateSettingInfo = GateSettings.Find(CurrentGate);
	if (ShootingAnimInstance && GateSettingInfo)
	{
		ShootingAnimInstance->ReceiveCurrentGate(CurrentGate);
		GetCharacterMovement()->MaxWalkSpeed = GateSettingInfo->MaxWalkSpeed;
		GetCharacterMovement()->MaxAcceleration = GateSettingInfo->MaxAcceleration;
		GetCharacterMovement()->BrakingDecelerationWalking = GateSettingInfo->BrakingDecelerationWalking;
		GetCharacterMovement()->BrakingFrictionFactor = GateSettingInfo->BrakingFrictionFactor;
		GetCharacterMovement()->BrakingFriction = GateSettingInfo->BrakingFriction;
		GetCharacterMovement()->bUseSeparateBrakingFriction = GateSettingInfo->bUseSeparateBrakingFriction;
	}
}
