#include "GameplayTagContainer.h"
#include "../Enumerator/ItemEnum.h"

#include "RPGItemStruct.generated.h"
#pragma once

USTRUCT(BlueprintType)
struct FRPGItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag IdentifyingTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGrade ItemGrade;
};