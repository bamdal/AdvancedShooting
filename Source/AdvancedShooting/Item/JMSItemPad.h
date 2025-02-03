// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JMSItemPad.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class ADVANCEDSHOOTING_API AJMSItemPad : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* ItemPad;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* ItemPadWeapon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UNiagaraComponent* ItemPadNiagaraEffect;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCapsuleComponent* ItemPadCapsule;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	UDataTable* ItemPadDataTable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Item")
	FDataTableRowHandle ItemHandle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FLinearColor HealthLinearColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FLinearColor ShieldLinearColor = FColor::Cyan;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FLinearColor PistolLinearColor = FColor::Magenta;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FLinearColor RifleLinearColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int32 Amount;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<UStaticMesh> HealthPad;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<UStaticMesh> ShieldPad;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<UStaticMesh> PistolPad;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<UStaticMesh> RiflePad;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<USkeletalMesh> PistolMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<USkeletalMesh> RifleMesh;


	UFUNCTION(BlueprintCallable)
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
public:	
	// Sets default values for this actor's properties
	AJMSItemPad();
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
