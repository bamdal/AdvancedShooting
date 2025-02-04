// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedShooting/Interface/BPI_Kraken.h"
#include "GameFramework/Character.h"
#include "JMSKraken.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UJMSKrakenHealthUI;

UCLASS()
class ADVANCEDSHOOTING_API AJMSKraken : public ACharacter, public IBPI_Kraken
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJMSKraken();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	float DamageReceived;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UJMSKrakenHealthUI> WBP_HealthBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	class UJMSKrakenHealthUI* HealthBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> DamageNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KrakenDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health, meta = (AllowPrivateAccess = "true"))
	int32 DamageAmount;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateHealthBar();

	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void Damage_Implementation(FName HitBoneName, FVector ImpactLocation) override;
	virtual void Attack_Implementation() override;

	void AttackProc();

	void JMSPlayMontageOnCompleted(UAnimMontage* Montage,FOnMontageEnded MontageEnded) const;
	FOnMontageEnded AttackMontageEnded;
	
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	
	
};
