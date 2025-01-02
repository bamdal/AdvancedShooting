#pragma once

#include "CoreMinimal.h"

// 들고있는 총 상태
UENUM(BlueprintType)
enum class E_Guns : uint8
{
	UnArmed		UMETA(DisplayName = "Unarmed"),
	Pistol		UMETA(DisplayName = "Pistol"),
	Rifle		UMETA(DisplayName = "Rifle"),
};

// 걷기 상대
UENUM(BlueprintType)
enum class E_Gate : uint8
{
	Walking		UMETA(DisplayName = "Walking"),
	Jogging		UMETA(DisplayName = "Jogging"),
	Crouch		UMETA(DisplayName = "Crouch"),
};