// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedShooting/Interface/WBPInterface.h"
#include "Blueprint/UserWidget.h"
#include "JMSWeaponUI.generated.h"

class UWrapBox;
/**
 * 
 */
UCLASS()
class ADVANCEDSHOOTING_API UJMSWeaponUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	UWrapBox* WrapBoxBullet;

	UPROPERTY(Meta = (BindWidget))
	UWrapBox* WrapBoxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PistolBulletWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PistolClipWidget;
	
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateBulletAmount(float Amount);

	UFUNCTION(BlueprintCallable)
	void UpdateClipAmount(float Amount);
};
