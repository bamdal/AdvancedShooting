#pragma once

#include "CoreMinimal.h"
#include "GateSetting.generated.h"

USTRUCT(BlueprintType)
struct FGateSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAcceleration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingDeceleration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFrictionFactor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFiction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseSeparateBrakingFiction;
};
