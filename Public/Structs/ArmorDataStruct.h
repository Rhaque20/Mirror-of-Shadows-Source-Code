#include "RPGItemStruct.h"
#include "Structs/EquipmentStatStruct.h"
#include "ArmorDataStruct.generated.h"
#pragma once

USTRUCT(BlueprintType)
struct FArmorData : public FRPGItemData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid ArmorGUID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EquipmentSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MainStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MainStatValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FEquipmentStat> Substats;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGrade EquipmentGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentEXP;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Level;

	bool operator==(const FArmorData& Other) const
	{
		return ArmorGUID == Other.ArmorGUID;
	}
};