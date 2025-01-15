// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSShootingAnimInstance.h"

#include "JMSShootingChar.h"
#include "KismetAnimationLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void FHAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::InitializeObjects(InAnimInstance);

	Owner = InAnimInstance->TryGetPawnOwner();
	if (Owner == nullptr)
	{
		return;
	}

	MovementComponent = Cast<UCharacterMovementComponent>(Owner->GetMovementComponent());
}

void FHAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	FAnimInstanceProxy::PreUpdate(InAnimInstance, DeltaSeconds);

	if (::IsValid(Owner))
	{
		ActorLocation = Owner->GetActorLocation();
		ActorRotation = Owner->GetActorRotation();
		AimRotationPitch = Owner->GetBaseAimRotation().Pitch;
	}
	if (::IsValid(MovementComponent))
	{
		GateSetting.MaxWalkSpeed = MovementComponent->MaxWalkSpeed;
		GateSetting.MaxAcceleration = MovementComponent->MaxAcceleration;
		GateSetting.BrakingDecelerationWalking = MovementComponent->BrakingDecelerationWalking;
		GateSetting.BrakingFrictionFactor = MovementComponent->BrakingFrictionFactor;
		GateSetting.BrakingFriction = MovementComponent->BrakingFriction;
		GateSetting.GroundFriction = MovementComponent->GroundFriction;
		GateSetting.bUseSeparateBrakingFriction = MovementComponent->bUseSeparateBrakingFriction;

		Velocity = MovementComponent->Velocity;
		CurrentAcceleration = MovementComponent->GetCurrentAcceleration();
		MovementMode = MovementComponent->MovementMode;
	}
}

void FHAnimInstanceProxy::Update(float DeltaSeconds)
{
	FAnimInstanceProxy::Update(DeltaSeconds);
}

void FHAnimInstanceProxy::PostUpdate(UAnimInstance* InAnimInstance) const
{
	FAnimInstanceProxy::PostUpdate(InAnimInstance);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UJMSShootingAnimInstance::UJMSShootingAnimInstance()
{
}

void UJMSShootingAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	HGetVelocity();
	HGetAccelerationData();
	HGetLocationData();
	HGetRotationData(DeltaSeconds);
	HUpdateOrientationData();
	HGetCharacterState(DeltaSeconds);
	HUpdateRootYawOffset(DeltaSeconds);
}

void UJMSShootingAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	MainChar = Cast<AJMSShootingChar>(GetOwningActor());
	if (MainChar)
	{
		HalfHeight = MainChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}
}

void UJMSShootingAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (HIsFalling)
	{
		if (MainChar)
		{
			const FVector start = MainChar->GetActorLocation() - FVector(0, 0, HalfHeight);
			const FVector end = MainChar->GetActorLocation() - FVector(0, 0, 1000);
			const TArray<AActor*> ActorsToIgnore;
			FHitResult OutHit;

			EDrawDebugTrace::Type DrawDebugTrace = EDrawDebugTrace::None;
			if (JMSDebugOption.ShowJumpData)
			{
				DrawDebugTrace = EDrawDebugTrace::ForOneFrame;
			}

			// bool const bHit = World ? World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(Radius), Params) : false;
			// 테스트 용도니 Kismet을 썻지만 Sweep 으로 해야함 
			const bool hit = UKismetSystemLibrary::SphereTraceSingle(this, start, end, 5.0f, TraceTypeQuery1, false,
			                                                         ActorsToIgnore,
			                                                         DrawDebugTrace, OutHit, true);
			if (hit)
			{
				ReceiveGroundDistance(OutHit.Distance);
			}
		}
	}
}

void UJMSShootingAnimInstance::HGetVelocity()
{
	HCharacterVelocity = HProxy.Velocity;
	HCharacterVelocity2D = FVector(HCharacterVelocity.X, HCharacterVelocity.Y, 0);
}

void UJMSShootingAnimInstance::HGetAccelerationData()
{
	HAcceleration = HProxy.CurrentAcceleration;
	HAcceleration2D = FVector(HAcceleration.X, HAcceleration.Y, 0);
	float Length = HAcceleration2D.Size2D();
	bool IsNearly = FMath::IsNearlyEqual(Length, 0.0f, 0.000001);
	HIsAcceleration = !IsNearly;
}

void UJMSShootingAnimInstance::HGetLocationData()
{
	HLastFrameWorldLocation = HWorldLocation;
	HWorldLocation = HProxy.ActorLocation;
	FVector Delta = HWorldLocation - HLastFrameWorldLocation;
	HDeltaLocation = Delta.Length();
}

void UJMSShootingAnimInstance::HGetRotationData(float DeltaSeconds)
{
	HLastFrameActorYaw = HActorYaw;
	HWorldRotation = HProxy.ActorRotation;
	HActorYaw = HWorldRotation.Yaw;
	HDeltaActorYaw = HActorYaw - HLastFrameActorYaw;
	if (DeltaSeconds > 0.0f)
	{
		const float MagicVal = 5.0f;
		float CalcVal = (HDeltaActorYaw / DeltaSeconds) / MagicVal;
		float Multiplier = 0.0f;
		if (HVelocityLocomotionDirection == E_LocomotionDirection::Forward)
		{
			Multiplier = 1.0f;
		}
		else if (HVelocityLocomotionDirection == E_LocomotionDirection::Backward)
		{
			Multiplier = -1.0f;
		}
		HLeanAngle = CalcVal * Multiplier;
		if (HLeanAngle < -90.0f)
		{
			HLeanAngle = -90.0f;
		}
		else if (HLeanAngle > 90.0f)
		{
			HLeanAngle = 90.0f;
		}

		
	}
	// 에임 오프셋 pitch값
	HAimPitch = FRotator::NormalizeAxis(HProxy.AimRotationPitch);
}

void UJMSShootingAnimInstance::HUpdateOrientationData()
{
	HLastFrameVelocityLocomotionDirection = HVelocityLocomotionDirection;
	HVelocityLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(HCharacterVelocity2D, HWorldRotation);

	HVelocityLocomotionAngleWithOffset = FRotator::NormalizeAxis(HVelocityLocomotionAngle - HRootYawOffset);

	HAccelerationLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(HAcceleration2D, HWorldRotation);

	HVelocityLocomotionDirection =
		HCalculateLocomotionDirection(HVelocityLocomotionAngle, HVelocityLocomotionDirection);
	HAccelerationLocomotionDirection = HCalculateLocomotionDirection(HAccelerationLocomotionAngle,
	                                                                 HAccelerationLocomotionDirection);
}

E_LocomotionDirection UJMSShootingAnimInstance::HCalculateLocomotionDirection(
	float CurrentLocomotionAngle, E_LocomotionDirection CurrentDirection,
	float BackwardMin, float BackwardMax, float ForwardMin, float ForwardMax, float DeadZone)
{
	switch (CurrentDirection)
	{
	case E_LocomotionDirection::Forward:
		{
			float Min = ForwardMin - DeadZone;
			float Max = ForwardMax + DeadZone;
			if (CurrentLocomotionAngle >= Min && CurrentLocomotionAngle <= Max)
			{
				return E_LocomotionDirection::Forward;
			}
		}
		break;
	case E_LocomotionDirection::Backward:
		{
			float Min = BackwardMin + DeadZone;
			float Max = BackwardMax - DeadZone;
			if (CurrentLocomotionAngle < Min || CurrentLocomotionAngle > Max)
			{
				return E_LocomotionDirection::Backward;
			}
		}
		break;
	case E_LocomotionDirection::Right:
		{
			float Min = ForwardMax - DeadZone;
			float Max = BackwardMax + DeadZone;
			if (CurrentLocomotionAngle >= Min && CurrentLocomotionAngle <= Max)
			{
				return E_LocomotionDirection::Right;
			}
		}
		break;
	case E_LocomotionDirection::Left:
		{
			float Min = BackwardMin - DeadZone;
			float Max = ForwardMin + DeadZone;
			if (CurrentLocomotionAngle >= Min && CurrentLocomotionAngle <= Max)
			{
				return E_LocomotionDirection::Left;
			}
		}
		break;

	default:
		break;
	}

	if (CurrentLocomotionAngle < BackwardMin || CurrentLocomotionAngle > BackwardMax)
	{
		return E_LocomotionDirection::Backward;
	}
	else if (CurrentLocomotionAngle >= ForwardMin && CurrentLocomotionAngle <= ForwardMax)
	{
		return E_LocomotionDirection::Forward;
	}
	else if (CurrentLocomotionAngle > 0.0f)
	{
		return E_LocomotionDirection::Right;
	}

	return E_LocomotionDirection::Left;
}

void UJMSShootingAnimInstance::HGetCharacterState(float DeltaSeconds)
{
	HGateSetting.MaxWalkSpeed = HProxy.GateSetting.MaxWalkSpeed;
	HGateSetting.MaxAcceleration = HProxy.GateSetting.MaxAcceleration;
	HGateSetting.BrakingDecelerationWalking = HProxy.GateSetting.BrakingDecelerationWalking;
	HGateSetting.BrakingFrictionFactor = HProxy.GateSetting.BrakingFrictionFactor;
	HGateSetting.BrakingFriction = HProxy.GateSetting.BrakingFriction;
	HGateSetting.GroundFriction = HProxy.GateSetting.GroundFriction;
	HGateSetting.bUseSeparateBrakingFriction = HProxy.GateSetting.bUseSeparateBrakingFriction;

	LastFrameGate = CurrentGate;
	CurrentGate = InComingGate;
	IsGateChanged = (CurrentGate != LastFrameGate) ? true : false;


	HLastFrameIsCrouched = HIsCrouching;
	HIsCrouching = (InComingGate == E_Gate::Crouch) ? true : false;
	if (HLastFrameIsCrouched != HIsCrouching)
	{
		HCrouchStateChanged = true;
	}
	else
	{
		HCrouchStateChanged = false;
	}

	// 점프 관련 처리 추가
	if (HProxy.MovementMode == MOVE_Falling)
	{
		HIsOnAIr = true;
	}
	else
	{
		HIsOnAIr = false;
	}

	if (HIsOnAIr && HCharacterVelocity.Z > 0.0f)
	{
		HIsJumping = true;
	}
	else
	{
		HIsJumping = false;
	}

	if (HIsOnAIr && HCharacterVelocity.Z < 0.0f)
	{
		HIsFalling = true;
	}
	else
	{
		HIsFalling = false;
	}

	// 캐릭터가 정점 또는 최대 높이에 도달했을 때
	if (HIsJumping)
	{
		float GravityZ = HProxy.MovementComponent->GetGravityZ();
		HTimeToJumpApex = (0 - HCharacterVelocity.Z) / GravityZ * HProxy.MovementComponent->GravityScale;
	}
	else
	{
		HTimeToJumpApex = 0.0f;
	}

	// 떨어지는 시간
	if (HIsFalling)
	{
		HTimeFalling += DeltaSeconds;
	}
	else
	{
		if (HIsJumping)
		{
			HTimeFalling = 0.0f;
		}
	}
}

void UJMSShootingAnimInstance::HUpdateRootYawOffset(float DeltaSeconds)
{
	if (HRootYawOffsetMode == E_RootYawOffsetMode::Accumulate)
	{
		float Angle = HRootYawOffset + (HDeltaActorYaw * -1.0f);
		HSetRootYawOffset(Angle);
	}
	else if (HRootYawOffsetMode == E_RootYawOffsetMode::BlendOut)
	{
		HSetRootYawOffset(0.0f);
		float Angle = UKismetMathLibrary::FloatSpringInterp(HRootYawOffset, 0.0f,
		                                                    HRootYawOffsetSpringState, 80.0f, 1.0f, DeltaSeconds, 1.0f,
		                                                    0.5f);
		HSetRootYawOffset(Angle);
	}

	HRootYawOffsetMode = E_RootYawOffsetMode::BlendOut;
}

void UJMSShootingAnimInstance::HSetRootYawOffset(float Angle)
{
	HRootYawOffset = FRotator::NormalizeAxis(Angle);
}

void UJMSShootingAnimInstance::HProcessTurnYawCurve()
{
	HLastFrameTurnYawCurveValue = HTurnYawCurveValue;

	FName IsTurning = FName("IsTurning");
	FName RootRotationZ = FName("root_rotation_Z");
	float CurrentIsTurning = GetCurveValue(IsTurning);
	if (CurrentIsTurning == 0.0f)
	{
		HTurnYawCurveValue = 0.0f;
		HLastFrameTurnYawCurveValue = 0.0f;
	}
	else
	{
		HTurnYawCurveValue = GetCurveValue(RootRotationZ) / CurrentIsTurning;
		if (HLastFrameTurnYawCurveValue != 0.0f)
		{
			float Delta = HRootYawOffset - (HTurnYawCurveValue - HLastFrameTurnYawCurveValue);
			HSetRootYawOffset(Delta);
		}
	}
}
