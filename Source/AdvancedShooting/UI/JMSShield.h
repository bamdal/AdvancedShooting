// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedShooting/Interface/WBPInterface.h"
#include "Blueprint/UserWidget.h"
#include "JMSShield.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class ADVANCEDSHOOTING_API UJMSShield : public UUserWidget, public IWBPInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ShieldAmount;
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void UpdateShieldWidget(float Amount) ;
	
	
};
