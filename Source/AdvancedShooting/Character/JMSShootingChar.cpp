// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSShootingChar.h"
#include "EnhancedInputComponent.h"
#include "JMSShootingAnimInstance.h"
#include "AdvancedShooting/Struct/WeaponSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "AdvancedShooting/UI/JMSCrosshair.h"
#include "AdvancedShooting/UI/JMSPistolUI.h"
#include "AdvancedShooting/UI/JMSRifleUI.h"
#include "AdvancedShooting/UI/JMSWeaponUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/SpotLightComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


AJMSShootingChar::AJMSShootingChar()
{
	PistolClipSize = 5;
	PistolClipAmount = 4;
	RifleClipSize = 20;
	RifleClipAmount = 4;



#pragma region 장비생성
	PistolBulletAmount = PistolClipSize;
	RifleBulletAmount = RifleClipSize;
	RifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh"));

	RifleMesh->SetupAttachment(GetMesh(), WeaponSockets.RifleUnEquipped);


	PistolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PistolMesh"));
	PistolMesh->SetupAttachment(GetMesh(), WeaponSockets.PistolUnEquipped);

	Helmet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Helmet"));
	Helmet->SetupAttachment(GetMesh(),TEXT("Helmet"));

	PistolHolster = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolHolster"));
	PistolHolster->SetupAttachment(GetMesh(),TEXT("PistolHolster"));
	
	Torch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch"));
	Torch->SetupAttachment(GetMesh(),TEXT("Torch"));

	TorchLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	TorchLightComponent->SetupAttachment(GetMesh(),TEXT("TorchLight"));
	TorchLightComponent->CastShadows
	

	TorchHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorchHolder"));
	TorchHolder->SetupAttachment(GetMesh(),TEXT("TorchHolder"));

	HealthBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetMesh(),TEXT("HealthBar"));

	Pistol_ReloadMontageEnded.BindUObject(this,&AJMSShootingChar::OnPistolReloadEnded);
	Rifle_ReloadMontageEnded.BindUObject(this,&ThisClass::OnRifleReloadEnded);

	PistolWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PistolWidget"));
	PistolWidgetComponent->SetupAttachment(PistolMesh,TEXT("Widget"));
	PistolWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	RifleWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("RifleWidget"));
	RifleWidgetComponent->SetupAttachment(RifleMesh,TEXT("Widget"));
	RifleWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
#pragma endregion	
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

	if (WBP_Crosshair)
	{
		CrosshairWidget = CreateWidget<UJMSCrosshair>(GetWorld(),WBP_Crosshair);
		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport();
			CrosshairWidget->PlayerController = GetLocalViewingPlayerController();
		}
		
		
	}
	

	FOnTimelineFloat AimDelegate;
	AimDelegate.BindUFunction(this,TEXT("OnAimUpdate"));

	// TimeLine에 함수 델리게이트로 연결
	AimTimeline.AddInterpFloat(AimCurveData, AimDelegate,TEXT("OnTimelineFloat"));

	if (PistolWidgetComponent)
	{
		
		PistolUI = Cast<UJMSPistolUI>(PistolWidgetComponent->GetWidget());

	}

	if (RifleWidgetComponent)
	{
		RifleUI = Cast<UJMSRifleUI>(RifleWidgetComponent->GetWidget());
	}

	PistolVisibleFunc(false);
	RifleVisibleFunc(false);
}

void AJMSShootingChar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	
	
	AimTimeline.TickTimeline(DeltaSeconds);

	if (EquippedWeapon == E_Weapon::Pistol)
	{
		CrosshairWidget->CrosshairPos = GetAimLocation(PistolMesh);
		
		
		
	}

	if (EquippedWeapon == E_Weapon::Rifle)
	{
		CrosshairWidget->CrosshairPos = GetAimLocation(RifleMesh);
	}
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
	UpdateGate(CurrentGate);
}

void AJMSShootingChar::AimStarted(const FInputActionValue& InputActionValue)
{
	if (EquippedWeapon == E_Weapon::Pistol && !IsStartReloading)
		PistolVisibleFunc(true);
	if (EquippedWeapon == E_Weapon::Rifle && !IsStartReloading)
		RifleVisibleFunc(true);
	if (CurrentGate == E_Gate::Walking || CurrentGate == E_Gate::Jogging)
	{
		UpdateGate(E_Gate::Walking);
	}
	else if (CurrentGate == E_Gate::Crouch)
	{
		UpdateGate(E_Gate::Crouch);
	}
	IsAiming = true;
	AimTimeline.PlayFromStart();
}

void AJMSShootingChar::AimCompleted(const FInputActionValue& InputActionValue)
{
	PistolVisibleFunc(false);
	RifleVisibleFunc(false);
	if (CurrentGate == E_Gate::Walking || CurrentGate == E_Gate::Jogging)
	{
		UpdateGate(E_Gate::Jogging);
		IsAiming = false;
	}
	else if (CurrentGate == E_Gate::Crouch)
	{
		UpdateGate(E_Gate::Crouch);
	}
	AimTimeline.ReverseFromEnd();
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
		IsAiming = false;
		UnCrouch();
	}
}

void AJMSShootingChar::StartFireAction(const FInputActionValue& InputActionValue)
{
	// 총알이 남아있을때만 수행하고 총알소비와 총알 바닥날 시 리로드, UI출력 

	if (IsCanFire && IsAiming)
	{
		if (EquippedWeapon == E_Weapon::Pistol)
		{
			IsCanFire = false;

			GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &ThisClass::FirePistol, PistolShootDelay,
			                                       true, 0);
		}

		if (EquippedWeapon == E_Weapon::Rifle)
		{
			IsCanFire = false;

			GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &ThisClass::FireRifle, RifleShootDelay,
			                                       true, 0);
		}
	}
}

void AJMSShootingChar::ResetIsCanFire()
{
	if (!IsStartReloading)
	{
		IsCanFire = true;
		IsResetIsCanFireFlag = true;
		
	}
}


void AJMSShootingChar::StopFireAction()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
	if (IsResetIsCanFireFlag)
	{
		float ShootDelay = EquippedWeapon == E_Weapon::Pistol ? PistolShootDelay : RifleShootDelay;
		GetWorld()->GetTimerManager().SetTimer(ShootingDelayTimerHandle, this, &ThisClass::ResetIsCanFire, ShootDelay,
		                                       false);
	}
	IsResetIsCanFireFlag = false;
}

void AJMSShootingChar::FirePistol()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red,TEXT("AJMSShootingChar PistolShoot"));
	bool HaveBullet = PistolBulletManager();
	if (HaveBullet)
	{
		
		JMSPlayMontage(PistolFireAnimMontage);
		JMSPlayAnimation(PistolMesh, PistolFireAnim, false);
		JMSPlaySound(PistolMesh, SoundPistolFire,TEXT("Barrel"));
		JMSFireLineTraceProc(PistolMesh);
	}
	else
	{
		IsResetIsCanFireFlag = true;
		StopFireAction();
	}
}

void AJMSShootingChar::FireRifle()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red,TEXT("AJMSShootingChar RifleShoot"));
	bool HaveBullet = RifleBulletManager();
	if (HaveBullet)
	{
		JMSPlayMontage(RifleFireAnimMontage);

		JMSPlayAnimation(RifleMesh, RifleFireAnim, false);
		JMSPlaySound(RifleMesh, SoundRifleFire,TEXT("Barrel"));
		JMSFireLineTraceProc(RifleMesh);
	}
	else
	{
		IsResetIsCanFireFlag = true;
		StopFireAction();
	}
}


void AJMSShootingChar::JMSFireLineTraceProc(USkinnedMeshComponent* Weapon)
{
	if (Weapon == nullptr)
		return;


	FTransform Trans = Weapon->GetSocketTransform(TEXT("Muzzle"), RTS_World);
	FVector Start = Trans.GetLocation();
	FVector End = Trans.GetLocation() + Trans.GetRotation().GetForwardVector() * 10000000;
	TArray<AActor*> ActorToIgnore;
	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceTypeQuery1, false, ActorToIgnore,
	                                                  EDrawDebugTrace::None, HitResult, true);
	if (bHit)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 0.3f, FColor::Purple,
		                                 FString::Printf(
			                                 TEXT("%s ,%s ,%s ,%s ,%s , "), *HitResult.ImpactPoint.ToString(),
			                                 *HitResult.ImpactNormal.ToString(), *HitResult.Normal.ToString(),
			                                 *HitResult.PhysMaterial->GetName(),
			                                 *HitResult.HitObjectHandle.GetName()));*/

		// 총알 충돌지점 별로 사운드 출력
		JMSImpactSound(HitResult.ImpactPoint, HitResult.PhysMaterial.Get());

		// 총 궤적 이펙트(나이아가라)
		JMSFireTraceEffect(HitResult.ImpactPoint);

		// 총알 피격 이펙트(믈리 머티리얼별 선별 처리)
		JMSFireImpactEffect(HitResult.PhysMaterial.Get(), HitResult.ImpactNormal, HitResult.ImpactPoint);
	}
	else
	{
		JMSFireTraceEffect(End);
	}
}

void AJMSShootingChar::JMSFireTraceEffect(FVector ImpactPoint)
{
	USceneComponent* AttachToComponent = nullptr;
	if (EquippedWeapon == E_Weapon::Pistol)
	{
		AttachToComponent = PistolMesh;
	}
	else if (EquippedWeapon == E_Weapon::Rifle)
	{
		AttachToComponent = RifleMesh;
	}

	if (AttachToComponent == nullptr || NSWeaponFire == nullptr)
		return;

	UNiagaraComponent* NComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NSWeaponFire, AttachToComponent,TEXT("Muzzle"), FVector::Zero(),
		FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);

	if (NComp == nullptr)
		return;

	TArray<FVector> ArrayData;
	ArrayData.Add(ImpactPoint);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NComp,TEXT("User.ImpactPositions"), ArrayData);
}

void AJMSShootingChar::JMSFireImpactEffect(UPhysicalMaterial* ImpactMaterial, FVector ImpactNormal, FVector ImpactPoint)
{
	if (ImpactMaterial == nullptr)
		return;

	UNiagaraSystem* NTemp;
	switch (ImpactMaterial->SurfaceType)
	{
	case SurfaceType1:
		{
			NTemp = NSImpactGlass;
			break;
		}
	case SurfaceType2:
		{
			NTemp = NSImpactConcrete;
			break;
		}
	default:
		{
			NTemp = NSImpactConcrete;
			break;;
		}
	}
	if (NTemp == nullptr)
		return;

	UNiagaraComponent* NComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, NTemp, FVector::Zero(), FRotator::ZeroRotator);
	if (NComp == nullptr)
		return;

	TArray<FVector> ArrayNormalData;
	ArrayNormalData.Add(ImpactNormal);

	TArray<FVector> ArrayPosData;
	ArrayPosData.Add(ImpactPoint);

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NComp,TEXT("User.ImpactNormals"), ArrayNormalData);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(NComp,TEXT("User.ImpactPositions"),
	                                                                   ArrayPosData);
}

void AJMSShootingChar::JMSImpactSound(FVector ImpactLocation, UPhysicalMaterial* ImpactMaterial)
{
	if (ImpactMaterial == nullptr)
		return;

	switch (ImpactMaterial->SurfaceType)
	{
	case SurfaceType1:
		{
			UGameplayStatics::PlaySoundAtLocation(this, MSPImpactsGlass, ImpactLocation);
			UGameplayStatics::PlaySoundAtLocation(this, MSPImpactsGlassDebris, ImpactLocation);
			break;
		}
	case SurfaceType2:
		{
			UGameplayStatics::PlaySoundAtLocation(this, MSPImpactsPlaster, ImpactLocation);
			UGameplayStatics::PlaySoundAtLocation(this, MSPImpactsPlasterDebris, ImpactLocation);
			break;
		}
	default:
		{
			UGameplayStatics::PlaySoundAtLocation(this, MSPImpactsPlaster, ImpactLocation);
			UGameplayStatics::PlaySoundAtLocation(this, MSPImpactsPlasterDebris, ImpactLocation);
			break;;
		}
	}
}


void AJMSShootingChar::ReloadAction()
{
	if (IsCanFire && !IsStartReloading)
	{
		StopFireAction();
		if (EquippedWeapon == E_Weapon::Pistol && PistolClipAmount > 0)
		{
			IsCanFire = false;
			IsResetIsCanFireFlag = false;
			JMSPlayMontageOnCompleted(PistolReloadAnimMontage,Pistol_ReloadMontageEnded);
			JMSPlayAnimation(PistolMesh, PistolReloadAnim, false);
			//JMSPlaySound(PistolMesh, SoundPistolReload,TEXT("Barrel"));
			IsStartReloading = true;

				PistolVisibleFunc(false);
			
			
		}
		if (EquippedWeapon == E_Weapon::Rifle && RifleReloadAnim && RifleClipAmount > 0)
		{
			IsCanFire = false;
			IsResetIsCanFireFlag = false;
			//JMSPlayMontage(RifleReloadAnimMontage);
			JMSPlayMontageOnCompleted(RifleReloadAnimMontage,Rifle_ReloadMontageEnded);
			JMSPlayAnimation(RifleMesh, RifleReloadAnim, false);
			//JMSPlaySound(RifleMesh, SoundRifleReload,TEXT("Barrel"));
			IsStartReloading = true;

			RifleVisibleFunc(false);
		}
	}
}


void AJMSShootingChar::OnPistolReloadEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsCanFire = true;
	IsResetIsCanFireFlag = true;
	IsStartReloading = false;
	if (PistolClipAmount > 0)
	{
		PistolClipAmount -= 1;
		PistolUI->UpdateClipAmount(PistolClipAmount);
		PistolBulletAmount = PistolClipSize;
		PistolUI->UpdateBulletAmount(PistolBulletAmount);

	}
	if (IsAiming && EquippedWeapon == E_Weapon::Pistol)
		PistolVisibleFunc(true);

	if (EquippedWeapon == E_Weapon::Rifle)
		RifleVisibleFunc(true);
}

void AJMSShootingChar::OnRifleReloadEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsCanFire = true;
	IsResetIsCanFireFlag = true;
	IsStartReloading = false;
	if (RifleClipAmount > 0)
	{
		RifleClipAmount -= 1;
		PistolUI->UpdateClipAmount(RifleClipAmount);
		RifleBulletAmount = RifleClipSize;
		RifleUI->UpdateBulletAmount(RifleBulletAmount);

	}
	if (IsAiming && EquippedWeapon == E_Weapon::Rifle)
		RifleVisibleFunc(true);
	if (EquippedWeapon == E_Weapon::Pistol)
		PistolVisibleFunc(true);
}


void AJMSShootingChar::ChangeWeapon(E_Weapon Equipped)
{
	StopFireAction();
	PistolVisibleFunc(false);
	RifleVisibleFunc(false);
	FName PistolAttachSocketName = WeaponSockets.PistolUnEquipped;
	FName RifleAttachSocketName = WeaponSockets.RifleUnEquipped;
	if (Equipped == E_Weapon::Pistol)
	{
		PistolAttachSocketName = WeaponSockets.WeaponEquipped;
		if (IsAiming && !IsStartReloading)
			PistolVisibleFunc(true);
	}
	else if (EquippedWeapon == E_Weapon::Rifle)
	{
		RifleAttachSocketName = WeaponSockets.WeaponEquipped;
		if (IsAiming&& !IsStartReloading)
			RifleVisibleFunc(true);
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

	/*FGateSetting* GateSettingInfo = GateSettings.Find(CurrentGate);
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
	}*/

	if (GateSettingTable != nullptr)
	{
		FGateSetting* Row = nullptr;
		bool IsCrouched = false;
		switch (CurrentGate)
		{
		case E_Gate::Walking:
			{
				switch (EquippedWeapon)
				{
				case E_Weapon::UnArmed:
					{
						Row = GateSettingTable->FindRow<FGateSetting>(TEXT("UnarmedWalk"), FString(""));
						break;
					}
				case E_Weapon::Pistol:
					{
						Row = GateSettingTable->FindRow<FGateSetting>(TEXT("PistolWalk"), FString(""));
						break;
					}
				case E_Weapon::Rifle:
					{
						Row = GateSettingTable->FindRow<FGateSetting>(TEXT("RifleWalk"), FString(""));
						break;
					}
				default: break;
				}
			}
			break;
		case E_Gate::Jogging:
			{
				switch (EquippedWeapon)
				{
				case E_Weapon::UnArmed:
					{
						Row = GateSettingTable->FindRow<FGateSetting>(TEXT("UnarmedJog"), FString(""));
						break;
					}
				case E_Weapon::Pistol:
					{
						Row = GateSettingTable->FindRow<FGateSetting>(TEXT("PistolJog"), FString(""));
						break;
					}
				case E_Weapon::Rifle:
					{
						Row = GateSettingTable->FindRow<FGateSetting>(TEXT("RifleJog"), FString(""));
						break;
					}
				default: break;
				}
			}
			break;
		case E_Gate::Crouch:
			{
				IsCrouched = true;
				switch (EquippedWeapon)
				{
				case E_Weapon::UnArmed:
					{
						Row = GateSettingTable->FindRow<FGateSetting>(TEXT("UnarmedCrouch"), FString(""));
						break;
					}
				case E_Weapon::Pistol:
					{
						Row = GateSettingTable->FindRow<FGateSetting>(TEXT("PistolCrouch"), FString(""));
						break;
					}
				case E_Weapon::Rifle:
					{
						Row = GateSettingTable->FindRow<FGateSetting>(TEXT("RifleCrouch"), FString(""));
						break;
					}
				default: break;
				}
			}
			break;
		default: break;
		}

		if (Row != nullptr)
		{
			ShootingAnimInstance->ReceiveCurrentGate(CurrentGate);
			GetCharacterMovement()->MaxWalkSpeed = Row->MaxWalkSpeed;
			if (IsCrouched)
				GetCharacterMovement()->MaxWalkSpeedCrouched = Row->MaxWalkSpeed;
			GetCharacterMovement()->MaxAcceleration = Row->MaxAcceleration;
			GetCharacterMovement()->BrakingDecelerationWalking = Row->BrakingDecelerationWalking;
			GetCharacterMovement()->BrakingFrictionFactor = Row->BrakingFrictionFactor;
			GetCharacterMovement()->BrakingFriction = Row->BrakingFriction;
			GetCharacterMovement()->GroundFriction = Row->GroundFriction;
			GetCharacterMovement()->bUseSeparateBrakingFriction = Row->bUseSeparateBrakingFriction;
		}
	}
}

void AJMSShootingChar::JMSPlayMontage(UAnimMontage* Montage)
{
	if (Montage == nullptr || AnimInstance == nullptr)
		return;

	AnimInstance->Montage_Play(Montage);
}


void AJMSShootingChar::JMSPlayMontageOnCompleted(UAnimMontage* Montage, FOnMontageEnded MontageEndedDelegate) const
{
	if (Montage == nullptr || AnimInstance == nullptr)
		return;
	
	AnimInstance->Montage_Play(Montage);
	// 몽타주 재생이 끝나면 델리게이트를 사용해 바인딩된 함수를 호출
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate,Montage);
	
}


void AJMSShootingChar::JMSPlayAnimation(USkeletalMeshComponent* Weapon, UAnimationAsset* AnimSequence, bool bLoop)
{
	if (Weapon == nullptr || AnimSequence == nullptr)
		return;
	Weapon->PlayAnimation(AnimSequence, bLoop);
}

void AJMSShootingChar::JMSPlaySound(USkinnedMeshComponent* Weapon, USoundBase* Sound, FName BoneName)
{
	if (Weapon == nullptr || Sound == nullptr)
		return;

	FTransform SpawnTransform = Weapon->GetSocketTransform(BoneName, RTS_World);
	UGameplayStatics::PlaySoundAtLocation(this, Sound, SpawnTransform.GetLocation());
	/*GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,
	                                 FString::Printf(TEXT("AJMSShootingChar Sound %s"), *Sound->GetName()));*/
}

void AJMSShootingChar::OnAimUpdate(float Alpha)
{
	float ZoomOut = FMath::Max(GetCameraBoom()->TargetArmLength, DesiredTargetArmLengthZoomOut);
	float ZoomIn = FMath::Min(GetCameraBoom()->TargetArmLength, DesiredTargetArmLengthZoomIn);


	float CurrentLength = FMath::Lerp(ZoomOut, ZoomIn, Alpha);
	GetCameraBoom()->TargetArmLength = CurrentLength;
}

void AJMSShootingChar::PistolVisibleFunc(bool IsVisible) const
{
	if (PistolUI)
	{
		PistolUI->UpdateBulletAmount(PistolBulletAmount);
		PistolUI->UpdateClipAmount(PistolClipAmount);
	}
	
	if (PistolWidgetComponent != nullptr)
	{
		if (PistolUI)
		{
			PistolUI->SetVisibility(ESlateVisibility::Collapsed);
			if (IsVisible)
			{
				PistolUI->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void AJMSShootingChar::RifleVisibleFunc(bool IsVisible) const
{
	if (RifleUI)
	{
		RifleUI->UpdateBulletAmount(RifleBulletAmount);
		RifleUI->UpdateClipAmount(RifleClipAmount);
	}
	
	if (PistolWidgetComponent != nullptr)
	{
		if (RifleUI)
		{
			RifleUI->SetVisibility(ESlateVisibility::Collapsed);
			if (IsVisible)
			{
				RifleUI->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

bool AJMSShootingChar::PistolBulletManager()
{
	if (PistolBulletAmount > 0)
	{
		PistolBulletAmount -= 1;
		PistolUI->UpdateBulletAmount(PistolBulletAmount);
		
		return true;
	}
	if (PistolClipAmount > 0)
	{
		return false;
	}

	return false;
}

bool AJMSShootingChar::RifleBulletManager()
{
	if (RifleBulletAmount > 0)
	{
		RifleBulletAmount -= 1;
		RifleUI->UpdateBulletAmount(RifleBulletAmount);
		return true;
	}
	if (RifleClipAmount > 0)
	{
		return false;
	}
	return false;
}
FVector AJMSShootingChar::GetAimLocation(USkinnedMeshComponent* Weapon)
{

	if (Weapon == nullptr)
		return FVector::ZeroVector;
	
	FTransform Trans = Weapon->GetSocketTransform(TEXT("Muzzle"), RTS_World);
	FVector Start = Trans.GetLocation();
	FVector End = Trans.GetLocation() + Trans.GetRotation().GetForwardVector() * 10000000;
	TArray<AActor*> ActorToIgnore;
	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceTypeQuery1, false, ActorToIgnore,
													  EDrawDebugTrace::None, HitResult, true);
	if (bHit)
	{
		return HitResult.ImpactPoint;
	}
	return End;
}