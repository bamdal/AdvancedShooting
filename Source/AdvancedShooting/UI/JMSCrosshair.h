// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "JMSCrosshair.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDSHOOTING_API UJMSCrosshair : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CrosshairPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerController* PlayerController;

	UPROPERTY()
	FVector2D CrosshairScreenPos;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* Crosshair;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
