// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "JMSCharBase.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ADVANCEDSHOOTING_API AJMSCharBase : public ACharacter
{
	GENERATED_BODY()

	// AllowPrivateAccess Private변수도 BP에서 수정가능
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE class UCameraComponent* GetFollowCamera() const {return FollowCamera;}

	// IMC & IA ----------------------------------------------------
public:
	/** MappingContext for player input. */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Asset;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Look;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Jump;

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	// IMC & IA ----------------------------------------------------

	
public:
	AJMSCharBase();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	virtual void Tick(float DeltaTime) override;

};
