// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSKrakenHealthUI.h"

#include "Components/ProgressBar.h"


void UJMSKrakenHealthUI::UpdateHealth(float HealthPercent)
{
	ProgressHealthBarLeft->SetPercent(HealthPercent);
	ProgressHealthBarRight->SetPercent(HealthPercent);
}
