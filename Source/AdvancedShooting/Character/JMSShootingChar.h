// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JMSCharBase.h"
#include "AdvancedShooting/Enum/JMSEnum.h"
#include "AdvancedShooting/Struct/GateSetting.h"
#include "AdvancedShooting/Struct/WeaponSocket.h"
#include "Components/TimelineComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "JMSShootingChar.generated.h"


class USpotLightComponent;
class UJMSRifleUI;
class UJMSPistolUI;
class UJMSWeaponUI;
class UWidgetComponent;
class UJMSCrosshair;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gate")
	UDataTable* GateSettingTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ABP")
	TSubclassOf<UAnimInstance> ABP_Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ABP")
	TSubclassOf<UAnimInstance> ABP_Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ABP")
	TSubclassOf<UAnimInstance> ABP_Rifle;

	// 장비 매시 -----------------------------------------------
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> RifleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> PistolMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Helmet;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PistolHolster;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Torch;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpotLightComponent> TorchLightComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TorchHolder;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HealthBar;

protected:
	
	// UI -------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<UWidgetComponent> PistolWidgetComponent;

	UPROPERTY()
	UJMSPistolUI* PistolUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<UWidgetComponent> RifleWidgetComponent;
	
	UPROPERTY()
	UJMSRifleUI* RifleUI;


	// Weapon Shoot Property ---------------------------------------------

	UPROPERTY(EditAnywhere)
	FWeaponSocket WeaponSockets;

	UPROPERTY(EditAnywhere)
	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PistolShootDelay = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RifleShootDelay = 0.2f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCanFire = true;

	bool IsResetIsCanFireFlag = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle ShootingTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle ShootingDelayTimerHandle;

	// Weapon Reload Property -----------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> PistolReloadAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> RifleReloadAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimationAsset> PistolReloadAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	TObjectPtr<UAnimMontage> PistolFireAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	TObjectPtr<UAnimMontage> RifleFireAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	TObjectPtr<UAnimationAsset> PistolFireAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	TObjectPtr<UAnimationAsset> RifleFireAnim;

protected:
	FOnMontageEnded Pistol_ReloadMontageEnded;
	FOnMontageEnded Rifle_ReloadMontageEnded;

	UFUNCTION()
	void OnPistolReloadEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void OnRifleReloadEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void UpdateGate(E_Gate Gate);

	UFUNCTION(BlueprintCallable)
	void JMSPlayMontage(UAnimMontage* Montage);


	void JMSPlayMontageOnCompleted(UAnimMontage* Montage, FOnMontageEnded MontageEndedDelegate) const;
	

	UFUNCTION(BlueprintCallable)
	void JMSPlayAnimation(USkeletalMeshComponent* Weapon, UAnimationAsset* AnimSequence, bool bLoop);

	// 사운드 --------------------------------------

	UFUNCTION(BlueprintCallable)
	void JMSPlaySound(USkinnedMeshComponent* Weapon, USoundBase* Sound, FName BoneName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
	USoundBase* SoundPistolFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
	USoundBase* SoundRifleFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
	USoundBase* SoundPistolReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
	USoundBase* SoundRifleReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
	USoundBase* MSPImpactsPlaster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
	USoundBase* MSPImpactsPlasterDebris;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
	USoundBase* MSPImpactsGlass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
	USoundBase* MSPImpactsGlassDebris;

protected:
	// 조준 --------------------------------------------------

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Aim")
	UJMSCrosshair* CrosshairWidget;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Aim")
	TSubclassOf<UJMSCrosshair> WBP_Crosshair;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	bool IsAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	TObjectPtr<UCurveFloat> AimCurveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	float DesiredTargetArmLengthZoomIn = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	float DesiredTargetArmLengthZoomOut = 500.0f;

	UPROPERTY()
	FTimeline AimTimeline;

	UFUNCTION(BlueprintCallable)
	void OnAimUpdate(float Alpha);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FVector GetAimLocation(USkinnedMeshComponent* Weapon);

	// 발사 ------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
 	class UNiagaraSystem* NSWeaponFire;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Shooting")
	class UNiagaraSystem* NSImpactConcrete;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Shooting")
	class UNiagaraSystem* NSImpactGlass;
	UFUNCTION(BlueprintCallable)
	void JMSFireLineTraceProc(USkinnedMeshComponent* Weapon);
	
	
	UFUNCTION(BlueprintCallable)
	void JMSFireTraceEffect(FVector ImpactPoint);
	
	UFUNCTION(BlueprintCallable)
	void JMSFireImpactEffect(UPhysicalMaterial* ImpactMaterial, FVector ImpactNormal, FVector ImpactPoint);
	
	UFUNCTION(BlueprintCallable)
	void JMSImpactSound(FVector ImpactLocation,UPhysicalMaterial* ImpactMaterial);


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Clip")
	float PistolClipSize;		// 탄창에 들어가는 총알 최대 수

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Clip")
	float PistolClipAmount;		// 탄창의 개수

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Clip")
	float PistolBulletAmount;	// 탄창에 남은 총알 갯수

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Clip")
	float RifleClipSize;		// 탄창에 들어가는 총알 최대 수

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Clip")
	float RifleClipAmount;		// 탄창의 개수

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Clip")
	float RifleBulletAmount;	// 탄창에 남은 총알 갯수

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Clip")
	bool IsStartReloading = false;	// 리로드 동작 중인지 판단

	UFUNCTION(BlueprintCallable)
    void PistolVisibleFunc(bool IsVisible) const;
	
	UFUNCTION(BlueprintCallable)
    void RifleVisibleFunc(bool IsVisible) const;
protected:
	UFUNCTION(BlueprintCallable)
	bool PistolBulletManager();
	
	UFUNCTION(BlueprintCallable)
	bool RifleBulletManager();

	
};


