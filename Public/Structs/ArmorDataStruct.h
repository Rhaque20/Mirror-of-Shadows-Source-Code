#include "GameplayTagContainer.h"
#include "../Enumerator/ItemEnum.h"

#include "ArmorDataStruct.generated.h"
#pragma once

USTRUCT(BlueprintType)
struct FArmorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EquipmentSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MainStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MainStatValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> Substats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> SubstatRarities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGrade EquipmentGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentEXP;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Level;
};