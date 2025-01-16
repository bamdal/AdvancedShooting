// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSShootingChar.h"

#include "EnhancedInputComponent.h"
#include "JMSShootingAnimInstance.h"
#include "AdvancedShooting/Struct/WeaponSocket.h"
#include "GameFramework/CharacterMovementComponent.h"


AJMSShootingChar::AJMSShootingChar()
{
	RifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh"));

	RifleMesh->SetupAttachment(GetMesh(), WeaponSockets.RifleUnEquipped);


	PistolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PistolMesh"));
	PistolMesh->SetupAttachment(GetMesh(), WeaponSockets.PistolUnEquipped);

	IsCanFire = true;
}


void AJMSShootingChar::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();

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
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &AJMSShootingChar::CrouchAction);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &AJMSShootingChar::StartFireAction);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, this, &AJMSShootingChar::StopFireAction);
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &AJMSShootingChar::ReloadAction);
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
	ChangeWeapon(EquippedWeapon);
}

void AJMSShootingChar::AimStarted(const FInputActionValue& InputActionValue)
{
	UpdateGate(E_Gate::Walking);
}

void AJMSShootingChar::AimCompleted(const FInputActionValue& InputActionValue)
{
	UpdateGate(E_Gate::Jogging);
}

void AJMSShootingChar::CrouchAction(const FInputActionValue& InputActionValue)
{
	if (CurrentGate == E_Gate::Walking || CurrentGate == E_Gate::Jogging)
	{
		UpdateGate(E_Gate::Crouch);
		Crouch();
	}
	else
	{
		UpdateGate(E_Gate::Jogging);
		UnCrouch();
	}
}

void AJMSShootingChar::StartFireAction(const FInputActionValue& InputActionValue)
{
	if (IsCanFire && CurrentGate != E_Gate::Jogging)
	{
		if (EquippedWeapon == E_Weapon::Pistol)
		{
			IsCanFire = false;

			GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &ThisClass::FirePistol, PistolShootDelay, true, 0);
		}

		if (EquippedWeapon == E_Weapon::Rifle)
		{
			IsCanFire = false;

			GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &ThisClass::FireRifle, RifleShootDelay, true, 0);
		}
	}
}

void AJMSShootingChar::ResetIsCanFire()
{
	IsCanFire = true;
	IsResetIsCanFireFlag = true;
}

void AJMSShootingChar::StopFireAction()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
	if (IsResetIsCanFireFlag)
	{
		float ShootDelay = EquippedWeapon == E_Weapon::Pistol ? PistolShootDelay : RifleShootDelay;
		GetWorld()->GetTimerManager().SetTimer(ShootingDelayTimerHandle, this, &ThisClass::ResetIsCanFire, ShootDelay, false);
	}
	IsResetIsCanFireFlag = false;
}

void AJMSShootingChar::FirePistol()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red,TEXT("PistolShoot"));
	if (AnimInstance && PistolFireAnimMontage)
	{
		AnimInstance->Montage_Play(PistolFireAnimMontage);
	}
	if (PistolFireAnim)
		PistolMesh->PlayAnimation(PistolFireAnim, false);
}

void AJMSShootingChar::FireRifle()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red,TEXT("RifleShoot"));
	if (AnimInstance && RifleFireAnimMontage)
	{
		AnimInstance->Montage_Play(RifleFireAnimMontage);
	}
	if (RifleFireAnim)
		RifleMesh->PlayAnimation(RifleFireAnim, false);
}

void AJMSShootingChar::ReloadAction()
{
	if (IsCanFire)
	{
		IsCanFire = false;
		IsResetIsCanFireFlag = false;
		if (EquippedWeapon == E_Weapon::Pistol && PistolReloadAnimMontage && PistolReloadAnim)
		{
			AnimInstance->Montage_Play(PistolReloadAnimMontage);
			PistolMesh->PlayAnimation(PistolReloadAnim, false);
			GetWorld()->GetTimerManager().SetTimer(ShootingDelayTimerHandle, this, &ThisClass::ResetIsCanFire, 2.0f, false);
		}
		if (EquippedWeapon == E_Weapon::Rifle && RifleReloadAnimMontage && RifleReloadAnim)
		{
			AnimInstance->Montage_Play(RifleReloadAnimMontage);
			PistolMesh->PlayAnimation(RifleReloadAnim, false);
			GetWorld()->GetTimerManager().SetTimer(ShootingDelayTimerHandle, this, &ThisClass::ResetIsCanFire, 2.2f, false);
		}

	}

}

void AJMSShootingChar::ChangeWeapon(E_Weapon Equipped)
{
	StopFireAction();
	FName PistolAttachSocketName = WeaponSockets.PistolUnEquipped;
	FName RifleAttachSocketName = WeaponSockets.RifleUnEquipped;
	if (Equipped == E_Weapon::Pistol)
	{
		PistolAttachSocketName = WeaponSockets.WeaponEquipped;
	}
	else if (EquippedWeapon == E_Weapon::Rifle)
	{
		RifleAttachSocketName = WeaponSockets.WeaponEquipped;
	}

	RifleMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                             RifleAttachSocketName);
	PistolMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                              PistolAttachSocketName);
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
		GetCharacterMovement()->GroundFriction = GateSettingInfo->GroundFriction;
		GetCharacterMovement()->bUseSeparateBrakingFriction = GateSettingInfo->bUseSeparateBrakingFriction;
	}
}
