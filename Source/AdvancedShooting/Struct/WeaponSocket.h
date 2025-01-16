#pragma once

#include "CoreMinimal.h"
#include "WeaponSocket.generated.h"

USTRUCT(BlueprintType)
struct FWeaponSocket
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PistolUnEquipped = TEXT("PistolUnEquipped");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RifleUnEquipped = TEXT("RifleUnEquipped");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponEquipped = TEXT("WeaponEquipped");
};