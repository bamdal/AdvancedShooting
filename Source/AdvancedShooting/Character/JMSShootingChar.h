// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JMSCharBase.h"
#include "AdvancedShooting/Enum/JMSEnum.h"
#include "AdvancedShooting/Struct/GateSetting.h"
#include "AdvancedShooting/Struct/WeaponSocket.h"
#include "Components/TimelineComponent.h"
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

	// 입력 ------------------------------

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_SwitchWeapon;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Aim;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Crouch;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Fire;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Reload;

protected:
	// 애니메이션 --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	E_Weapon EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gate")
	E_Gate CurrentGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gate")
	TMap<E_Gate, FGateSetting> GateSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ABP")
	TSubclassOf<UAnimInstance> ABP_Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ABP")
	TSubclassOf<UAnimInstance> ABP_Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ABP")
	TSubclassOf<UAnimInstance> ABP_Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<USkeletalMeshComponent> RifleMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<USkeletalMeshComponent> PistolMesh;

	// Weapon Shoot Property ---------------------------------------------
	
	UPROPERTY(EditAnywhere)
	FWeaponSocket WeaponSockets;

	UPROPERTY(EditAnywhere)
	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float PistolShootDelay = 0.5f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RifleShootDelay = 0.2f;


	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsCanFire = true;

	bool IsResetIsCanFireFlag = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTimerHandle ShootingTimerHandle;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTimerHandle ShootingDelayTimerHandle;

	// Weapon Reload Property -----------------------------------------

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAnimMontage> PistolReloadAnimMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAnimMontage> RifleReloadAnimMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAnimationAsset> PistolReloadAnim;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAnimationAsset> RifleReloadAnim;
	
	

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	// 입력 ------------------------------

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SwitchWeapon(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void AimStarted(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void AimCompleted(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void CrouchAction(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void StartFireAction(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void ResetIsCanFire();
	
	UFUNCTION(BlueprintCallable)
	void StopFireAction();

	UFUNCTION(BlueprintCallable)
	void FirePistol();
	
	UFUNCTION(BlueprintCallable)
	void FireRifle();

	UFUNCTION(BlueprintCallable)
	void ReloadAction();
	
	UFUNCTION(BlueprintCallable)
	void ChangeWeapon(E_Weapon Equipped);
protected:
	// 애니메이션 --------------------------------------

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Shooting")
	TObjectPtr<UAnimMontage> PistolFireAnimMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Shooting")
	TObjectPtr<UAnimMontage> RifleFireAnimMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Shooting")
	TObjectPtr<UAnimationAsset> PistolFireAnim;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Shooting")
	TObjectPtr<UAnimationAsset> RifleFireAnim;

	UFUNCTION(BlueprintCallable)
	void UpdateGate(E_Gate Gate);

	UFUNCTION(BlueprintCallable)
	void JMSPlayMontage(UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable)
	void JMSPlayAnimation(USkeletalMeshComponent* Weapon, UAnimationAsset* AnimSequence, bool bLoop);

	// 사운드 --------------------------------------
	
	UFUNCTION(BlueprintCallable)
	void JMSPlaySound(USkinnedMeshComponent* Weapon, USoundBase* Sound, FName BoneName);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Shooting")
	USoundBase* SoundPistolFire;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Shooting")
	USoundBase* SoundRifleFire;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Shooting")
	USoundBase* SoundPistolReload;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Shooting")
	USoundBase* SoundRifleReload;

protected:
	// 조준 --------------------------------------------------
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Aim")
	bool IsAiming = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Aim")
	TObjectPtr<UCurveFloat> AimCurveData;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Aim")
	float DesiredTargetArmLengthZoomIn = 400.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Aim")
	float DesiredTargetArmLengthZoomOut = 500.0f;
	
	UPROPERTY()
	FTimeline AimTimeline;

	UFUNCTION(BlueprintCallable)
	void OnAimUpdate(float Alpha);

	// 발사 ------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void JMSFireLineTraceProc(USkinnedMeshComponent* Weapon);
};
