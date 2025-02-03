// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSKraken.h"


// Sets default values
AJMSKraken::AJMSKraken()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AJMSKraken::Damage_Implementation(FName HitBoneName, FVector ImpactLocation)
{
	//IBPI_Kraken::Damage_Implementation(HitBoneName, ImpactLocation);
}

void AJMSKraken::Attack_Implementation()
{
	//IBPI_Kraken::Attack_Implementation();
}


// Called when the game starts or when spawned
void AJMSKraken::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJMSKraken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJMSKraken::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

