// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JMSCharBase.h"
#include "AdvancedShooting/Enum/JMSEnum.h"
#include "AdvancedShooting/Interface/ABPas.h"
#include "AdvancedShooting/Struct/GateSetting.h"
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

protected:

	virtual void BeginPlay() override;
	
	// 입력 ------------------------------

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SwitchWeapon(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void AimStarted(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void AimCompleted(const FInputActionValue& InputActionValue);

protected:
	// 애니메이션 --------------------------------------

	UFUNCTION(BlueprintCallable)
	void UpdateGate(E_Gate Gate);

};
