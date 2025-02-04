// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMSKrakenHealthUI.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDSHOOTING_API UJMSKrakenHealthUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	class UProgressBar* ProgressHealthBarLeft;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	class UProgressBar* ProgressHealthBarRight;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float HealthPercent);
	
	
};
