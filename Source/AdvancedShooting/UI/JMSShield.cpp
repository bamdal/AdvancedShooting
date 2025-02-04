// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSShield.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"


void UJMSShield::UpdateShieldWidget_Implementation(float Amount)
{
	ShieldAmount->SetText(FText::AsNumber(Amount));
}
