#include "GameplayTagContainer.h"
#include "../Enumerator/ItemEnum.h"

#include "RPGItemStruct.generated.h"
#pragma once

USTRUCT(BlueprintType)
struct FRPGItemData
{
	GENERATED_BODY()
	FRPGItemData() {}
	
	// Copy constructor and operator overload.
	FRPGItemData(const FRPGItemData&) = default;
	FRPGItemData& operator = (const FRPGItemData&) = default;
	
	// Move constructor and operator overload.
	FRPGItemData(FRPGItemData&&) = default;
	FRPGItemData& operator = (FRPGItemData&&) = default;
	
	// Virtual destructor requires us to declare copy, member wise, and move constructors.
	virtual ~FRPGItemData() {}
};

uint32 GetTypeHash(const FRPGItemData& Thing);