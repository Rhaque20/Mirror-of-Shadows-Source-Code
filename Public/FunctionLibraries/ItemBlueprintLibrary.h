#include "ItemBlueprintLibrary.generated.h"
#pragma once

struct FEquipmentMainStat;
struct FEquipmentSubstatsStat;
struct FEquipmentStat;
struct FArmorSlotandSet;
struct FItemGrade;
struct FItemSpec;

UCLASS()
class UItemBlueprintLibrary :
	public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// UFUNCTION(BlueprintCallable, Category = "Spec Fragment")
	// static FItemSpec GenerateArmor(FGameplayTag SetTag, FGameplayTag SlotTag,EGrade Rarity);

	// UFUNCTION(BlueprintCallable, Category = "Spec Fragment")
	// static FEquipmentMainStat FindMainStatData(const FItemSpec& Spec, bool& bDidFind);
	//
	// UFUNCTION(BlueprintCallable, Category = "Spec Fragment")
	// static FEquipmentSubstats FindSubStatData(const FItemSpec& Spec,bool& bDidFind);
	//
	// UFUNCTION(BlueprintCallable, Category = "Spec Fragment")
	// static FArmorSlotandSet FindSlotAndSetData(const FItemSpec& Spec,bool& bDidFind);
	
};