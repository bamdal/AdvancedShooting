#pragma once

#include "CoreMinimal.h"
#include "DebugOption.generated.h"

USTRUCT(BlueprintType)
struct FDebugOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowLocomotionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowGateData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DistanceMatching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowRootYawOffset;
};
