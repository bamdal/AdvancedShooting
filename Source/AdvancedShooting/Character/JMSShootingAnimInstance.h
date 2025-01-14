// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedShooting/Interface/ABPas.h"
#include "AdvancedShooting/Struct/DebugOption.h"
#include "AdvancedShooting/Struct/GateSetting.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "Kismet/KismetMathLibrary.h"
#include "JMSShootingAnimInstance.generated.h"

USTRUCT()
struct FHAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	virtual void InitializeObjects(UAnimInstance* InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;
	virtual void PostUpdate(UAnimInstance* InAnimInstance) const override;

public:
	UPROPERTY(Transient)
	TObjectPtr<class APawn> Owner;
	UPROPERTY(Transient)
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(Transient)
	FGateSetting GateSetting;

	UPROPERTY(Transient)
	FVector ActorLocation;

	UPROPERTY(Transient)
	FRotator ActorRotation;

	UPROPERTY(Transient)
	FVector Velocity;

	UPROPERTY(Transient)
	FVector CurrentAcceleration;
};

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HDebug)
	FDebugOption JMSDebugOption;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HGate)
	FGateSetting HGateSetting;

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

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HVelocityData)
	FVector HCharacterVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HVelocityData)
	FVector HCharacterVelocity2D;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HVelocityData)
	float HVelocityLocomotionAngle;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HAccelerationData)
	FVector HAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HAccelerationData)
	FVector HAcceleration2D;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HAccelerationData)
	bool HIsAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HAccelerationData)
	FVector HPivotAcceleration2D;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HLocationData)
	FVector HWorldLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HLocationData)
	FVector HLastFrameWorldLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HLocationData)
	float HDeltaLocation;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRotationData)
	FRotator HWorldRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRotationData)
	float HActorYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRotationData)
	float HLastFrameActorYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRotationData)
	float HDeltaActorYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRotationData)
	float HLeanAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRotationData)
	float HAccelerationLocomotionAngle;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HLocomotion)
	E_LocomotionDirection HVelocityLocomotionDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HLocomotion)
	E_LocomotionDirection HLastFrameVelocityLocomotionDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HLocomotion)
	E_LocomotionDirection HAccelerationLocomotionDirection;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRootYawOffset)
	float HRootYawOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRootYawOffset)
	E_RootYawOffsetMode HRootYawOffsetMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRootYawOffset)
	FFloatSpringState HRootYawOffsetSpringState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRootYawOffset)
	float HVelocityLocomotionAngleWithOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRootYawOffset)
	float HTurnYawCurveValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HRootYawOffset)
	float HLastFrameTurnYawCurveValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HCrouch)
	bool HIsCrouching;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HCrouch)
	bool HLastFrameIsCrouched;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HCrouch)
	bool HCrouchStateChanged;

	
public:
	
	// Interface ==========================================
	
	UFUNCTION(BlueprintCallable)
	virtual void ReceiveCurrentGate(E_Gate Gate) override { InComingGate = Gate; };

	UFUNCTION(BlueprintCallable)
	virtual void ReceiveEquippedGun(E_Weapon Gun) override { EquippedGun = Gun; };
	


	
private:
	UPROPERTY(Transient)
	FHAnimInstanceProxy HProxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return &HProxy;
	}
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override
	{
	}
	
public:
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void HGetVelocity();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void HGetAccelerationData();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void HGetLocationData();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void HGetRotationData(float DeltaSeconds);
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void HUpdateOrientationData();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	E_LocomotionDirection HCalculateLocomotionDirection(float CurrentLocomotionAngle, E_LocomotionDirection CurrentDirection,
		float BackwardMin = -130.0f, float BackwardMax = 130.0f, float ForwardMin = -50.0f, float ForwardMax = 50.0f, float DeadZone = 20.0f);
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void HGetCharacterState();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void HUpdateRootYawOffset(float DeltaSeconds);
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void HSetRootYawOffset(float Angle);
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void HProcessTurnYawCurve();
};
