#include "FunctionLibraries/ItemBlueprintLibrary.h"
#include "FunctionLibraries/ItemBlueprintLibrary.h"
#include "Structs/ItemSpec.h"
#include "GAS/UDGameplayTags.h"


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
