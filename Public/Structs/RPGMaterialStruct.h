#include "RPGItemStruct.h"

#include "RPGMaterialStruct.generated.h"
#pragma once

USTRUCT(BlueprintType)
struct FRPGMaterialData : public FRPGItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag IdentifyingTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGrade ItemGrade;
};