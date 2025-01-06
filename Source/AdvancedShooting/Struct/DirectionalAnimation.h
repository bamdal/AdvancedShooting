#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimSequence.h"
#include "DirectionalAnimation.generated.h"

USTRUCT(BlueprintType)
struct FDirectionalAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Forward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Backward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Right;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Left;

};
