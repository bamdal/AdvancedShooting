// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSWeaponUI.h"

#include "Components/WrapBox.h"



void UJMSWeaponUI::NativeConstruct()
{
	Super::NativeConstruct();
	WrapBoxBullet->ClearChildren();
	WrapBoxClip->ClearChildren();
}

void UJMSWeaponUI::UpdateBulletAmount(float Amount)
{
	if (WrapBoxBullet == nullptr ||  PistolBulletWidget == nullptr)
		return;
	WrapBoxBullet->ClearChildren();
	for (int i = 0; i < Amount; i++)
	{
		UWidget* wid = CreateWidget(this,PistolBulletWidget);
		if (wid == nullptr)
			return;
		WrapBoxBullet->AddChild(wid);
	}
}

void UJMSWeaponUI::UpdateClipAmount(float Amount)
{
	if (WrapBoxClip == nullptr ||PistolClipWidget == nullptr)
		return;
	
	WrapBoxClip->ClearChildren();
	for (int i = 0; i < Amount; i++)
	{
		UWidget* wid = CreateWidget(this,PistolClipWidget);
		if (wid == nullptr)
			return;
		WrapBoxClip->AddChild(wid);
	}
}

