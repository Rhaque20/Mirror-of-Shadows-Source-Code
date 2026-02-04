#include "GameplayTagContainer.h"
#include "../Enumerator/ItemEnum.h"

#include "RPGItemStruct.generated.h"
#pragma once

USTRUCT(BlueprintType)
struct FRPGItemData
{
	GENERATED_BODY()
};

uint32 GetTypeHash(const FRPGItemData& Thing);