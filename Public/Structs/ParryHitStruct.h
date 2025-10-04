#include "../Enumerator/ParryableHitTypeEnum.h"
#include "ParryHitStruct.generated.h"
#pragma once

USTRUCT(BlueprintType)
struct FParryHitType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanTriggerParry;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bWillInterruptOnParry;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	EParryableHitType HitType;

};