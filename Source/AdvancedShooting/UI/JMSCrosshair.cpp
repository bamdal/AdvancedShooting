// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSCrosshair.h"

#include "Blueprint/WidgetLayoutLibrary.h"


void UJMSCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PlayerController)
	{
		if(PlayerController->ProjectWorldLocationToScreen(CrosshairPos,CrosshairScreenPos,true))
		{
			FVector2D ViewportSize;
			if (GEngine && GEngine->GameViewport)
			{
				GEngine->GameViewport->GetViewportSize(ViewportSize);
			}
			
			// UI 스케일 가져오기
			float UIScale = UWidgetLayoutLibrary::GetViewportScale(this);
			ViewportSize /= UIScale;

			FVector2D AdjustedPos = CrosshairScreenPos / UIScale;
	
			// Border의 중심점을 기준으로 위치 설정
			FWidgetTransform WidgetTransform;
			WidgetTransform.Translation = AdjustedPos;

			Crosshair->SetRenderTransform(WidgetTransform);
		}
	}
}
