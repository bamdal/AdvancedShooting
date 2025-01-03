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

UENUM(BlueprintType)
enum class E_LocomotionDirections : uint8
{
	Forward		UMETA(DisplayName = "Forward"),
	Backward 	UMETA(DisplayName = "Backward"),
	Right 		UMETA(DisplayName = "Right"),
	Left 		UMETA(DisplayName = "Left"),
	
};