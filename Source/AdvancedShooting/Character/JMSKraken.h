// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedShooting/Interface/BPI_Kraken.h"
#include "GameFramework/Character.h"
#include "JMSKraken.generated.h"

UCLASS()
class ADVANCEDSHOOTING_API AJMSKraken : public ACharacter, public IBPI_Kraken
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJMSKraken();

	
	virtual void Damage_Implementation(FName HitBoneName, FVector ImpactLocation) override;
	virtual void Attack_Implementation() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
