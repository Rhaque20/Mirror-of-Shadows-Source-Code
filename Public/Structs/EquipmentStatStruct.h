
#include "GameplayTagContainer.h"
#include "EquipmentStatStruct.generated.h"

#pragma once

USTRUCT(BlueprintType)
struct FEquipmentStat
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayTag StatTag;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float StatValue;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int StatRarity;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bIsUnlocked = false;
};

uint32 GetTypeHash(const FEquipmentStat& Thing);