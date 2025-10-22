#include "FunctionLibraries/ItemBlueprintLibrary.h"
#include "FunctionLibraries/ItemBlueprintLibrary.h"
#include "Structs/ItemSpec.h"
#include "GAS/UDGameplayTags.h"

FItemSpec UItemBlueprintLibrary::GenerateArmor(FGameplayTag SetTag, FGameplayTag SlotTag, EGrade Rarity)
{
	FItemSpec Result = FItemSpec();
	
	// FArmorSlotandSet SlotAndSet = FArmorSlotandSet(SetTag,SlotTag);
	// Result.AddFragment<FArmorSlotandSet>(SlotAndSet);
	//
	// FEquipmentMainStat MainStat = FEquipmentMainStat(TAG_Stats_Flat_ATK,12.0f,1);;
	//
	// Result.AddFragment<FEquipmentMainStat>(MainStat);
	

	return Result;
}

// FEquipmentMainStat UItemBlueprintLibrary::FindMainStatData(const FItemSpec& Spec, bool& bDidFind)
// {
// 	const auto* Fragment = Spec.FindFragment<FEquipmentMainStat>();
// 	if (Fragment == nullptr)
// 	{
// 		bDidFind = false;
// 		return FEquipmentMainStat();
// 	}
//
// 	bDidFind = true;
// 	
// 	return *Fragment;
// }
//
// FEquipmentSubstats UItemBlueprintLibrary::FindSubStatData(const FItemSpec& Spec, bool& bDidFind)
// {
// 	const auto* Fragment = Spec.FindFragment<FEquipmentSubstats>();
// 	if (Fragment == nullptr)
// 	{
// 		bDidFind = false;
// 		return FEquipmentSubstats();
// 	}
//
// 	bDidFind = true;
// 	
// 	return *Fragment;
// }
//
// FArmorSlotandSet UItemBlueprintLibrary::FindSlotAndSetData(const FItemSpec& Spec, bool& bDidFind)
// {
// 	const auto* Fragment = Spec.FindFragment<FArmorSlotandSet>();
// 	if (Fragment == nullptr)
// 	{
// 		bDidFind = false;
// 		return FArmorSlotandSet();
// 	}
//
// 	bDidFind = true;
// 	
// 	return *Fragment;
// }
