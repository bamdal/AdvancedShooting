// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSKraken.h"

#include "JMSShootingChar.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "AdvancedShooting/UI/JMSKrakenHealthUI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AJMSKraken::AJMSKraken()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::AJMSKraken::OnMeshBeginOverlap);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetSphereRadius(4000.0f);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AJMSKraken::OnComponentBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::AJMSKraken::OnComponentEndOverlap);
}


// Called when the game starts or when spawned
void AJMSKraken::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	if (WBP_HealthBar != nullptr)
	{
		HealthBar = CreateWidget<UJMSKrakenHealthUI>(GetWorld(), WBP_HealthBar);
		if (HealthBar != nullptr)
		{
			HealthBar->AddToViewport();
			HealthBar->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	UpdateHealthBar();
	bIsDead = false;
}

// Called every frame
void AJMSKraken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJMSKraken::UpdateHealthBar()
{
	FVector2D InRange(0.0f, MaxHealth);
	FVector2D OutRange(0.0f, 1.0f);
	float HealthVal = FMath::GetMappedRangeValueClamped(InRange, OutRange, Health);
	if (HealthBar != nullptr)
	{
		HealthBar->UpdateHealth(HealthVal);
	}
}

void AJMSKraken::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!bIsAttacking || DamageAmount>3)
	{
		return;
	}
	
	if (OtherActor == nullptr)
		return;
	
	AJMSShootingChar* Char = Cast<AJMSShootingChar>(OtherActor);
	if (Char != nullptr)
	{
		Char->DecreaseHealth(FMath::RandRange(5,10));
		DamageAmount++;
		FVector pos = Char->GetActorLocation()-GetActorLocation();
		pos.Normalize();
		Char->LaunchCharacter(pos*1000.0f,true,false);
	}
	
}

void AJMSKraken::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;

	AJMSShootingChar* Char = Cast<AJMSShootingChar>(OtherActor);
	if (Char != nullptr)
	{
		if (HealthBar != nullptr)
		{
			HealthBar->SetVisibility(ESlateVisibility::Visible);
		}

		UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);
		if (BlackboardComponent != nullptr)
		{
			BlackboardComponent->SetValueAsObject(TEXT("Player"), Char);
		}
	}
}

void AJMSKraken::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr)
		return;

	AJMSShootingChar* Char = Cast<AJMSShootingChar>(OtherActor);
	if (Char != nullptr)
	{
		if (HealthBar != nullptr)
		{
			HealthBar->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AJMSKraken::Damage_Implementation(FName HitBoneName, FVector ImpactLocation)
{
	//IBPI_Kraken::Damage_Implementation(HitBoneName, ImpactLocation);

	if (bIsDead)
	{
		return;
	}
	if (HitBoneName == FName("KRAKEN_SHELL"))
	{
		DamageReceived = FMath::RandRange(3.0f, 10.0f);
	}
	else if (HitBoneName == FName("KRAKEN_BEAK_LEFT") ||
			 HitBoneName == FName("KRAKEN_BEAK_LOWER") ||
			 HitBoneName == FName("KRAKEN_BEAK_RIGHT") ||
			 HitBoneName == FName("KRAKEN_BEAK_UPPER"))
	{
		DamageReceived = FMath::RandRange(50.0f, 100.0f);
	}
	else
	{
		DamageReceived = FMath::RandRange(25.0f, 50.0f);
	}
	Health -= DamageReceived;
	bIsDead = Health <= 0.0f;
	UpdateHealthBar();

	if (DamageNumber)
	{
		UNiagaraComponent* NComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,DamageNumber,FVector::ZeroVector);
		if (NComp != nullptr)
		{
			FVector4 DamageData(ImpactLocation.X,ImpactLocation.Y,ImpactLocation.Z,DamageReceived);
			TArray<FVector4> ArrayData;
			ArrayData.Add(DamageData);
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(NComp,TEXT("User.DamageInfo"),ArrayData);
		}
	}

	if (bIsDead)
	{
		if (GetMesh() &&GetMesh()->GetAnimInstance() && KrakenDeath)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(KrakenDeath);
		}

		UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);
		if (BlackboardComponent != nullptr)
		{
			BlackboardComponent->SetValueAsBool(TEXT("IsDeadKey"), bIsDead);
		}
	}
	else
	{
		AJMSShootingChar* Char = Cast<AJMSShootingChar>(UGameplayStatics::GetPlayerCharacter(this,0));
		if (Char != nullptr)
		{
			if (HealthBar != nullptr)
			{
				HealthBar->SetVisibility(ESlateVisibility::Visible);
			}

			UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);
			if (BlackboardComponent != nullptr)
			{
				BlackboardComponent->SetValueAsObject(TEXT("Player"), Char);
			}
		}
	}
}

void AJMSKraken::Attack_Implementation()
{
	//IBPI_Kraken::Attack_Implementation();
	
	AttackProc();
}

void AJMSKraken::AttackProc()
{
	if (bIsDead)
	{
		return;
	}
	
	DamageAmount =0;
	bIsAttacking = true;

	int32 AttackTypeCount = AttackMontages.Num();
	if (AttackTypeCount >0)
	{
		int32 Index = FMath::RandRange(0,AttackTypeCount-1);
		JMSPlayMontageOnCompleted(AttackMontages[Index],AttackMontageEnded);
	}
	
}

void AJMSKraken::JMSPlayMontageOnCompleted(UAnimMontage* Montage, FOnMontageEnded MontageEnded) const
{
	if (Montage == nullptr)
		return;
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded,Montage);
	}
}

void AJMSKraken::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
}
