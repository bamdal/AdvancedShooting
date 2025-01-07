// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedShooting/Interface/ABPas.h"
#include "Animation/AnimInstance.h"
#include "JMSShootingAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDSHOOTING_API UJMSShootingAnimInstance : public UAnimInstance, public IABPas
{
	GENERATED_BODY()
	
public:

	UJMSShootingAnimInstance();

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Guns")
	E_Weapon EquippedGun;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Gate")
	E_Gate CurrentGate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Gate")
	E_Gate InComingGate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Gate")
	E_Gate LastFrameGate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Gate")
	bool IsGateChanged;

public:

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	
	// Interface ==========================================
	
	UFUNCTION(BlueprintCallable)
	virtual void ReceiveCurrentGate(E_Gate Gate) override { InComingGate = Gate; };

	UFUNCTION(BlueprintCallable)
	virtual void ReceiveEquippedGun(E_Weapon Gun) override { EquippedGun = Gun; };
	
	
};
