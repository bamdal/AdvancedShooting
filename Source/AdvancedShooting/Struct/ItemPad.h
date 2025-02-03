#pragma once

#include "CoreMinimal.h"
#include "AdvancedShooting/Enum/JMSEnum.h"
#include "Engine/DataTable.h"
#include "ItemPad.generated.h"


USTRUCT(BlueprintType)
struct FItemPad : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemPadType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};
