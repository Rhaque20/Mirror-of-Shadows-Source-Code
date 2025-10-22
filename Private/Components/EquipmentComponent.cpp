#include "Components/EquipmentComponent.h"
#include <GameplayEffect.h>
#include "GAS/UDGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"

void UEquipmentComponent::BeginPlay()
{
    Super::BeginPlay();
    IAbilitySystemInterface* ImplementedOwner = Cast<IAbilitySystemInterface>(GetOwner());

    if (ImplementedOwner)
    {
        OwningASC = ImplementedOwner->GetAbilitySystemComponent();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OWNER %s DOES NOT HAVE ABILITY SYSTEM COMPONENT"), *(GetOwner()->GetName()));
    }

    FArmorData dummyval;

    for (TMap<FGameplayTag, FArmorData>::TRangedForIterator Itr = EquippedArmor.begin(); Itr != EquippedArmor.end(); ++Itr)
    {
        EquipArmorInSlot(Itr->Value, dummyval);
    }
}

void UEquipmentComponent::CheckSetBonus()
{
    
}

void UEquipmentComponent::TurnArmorIntoStats(FArmorData ArmorEquipped)
{
    TSubclassOf<UGameplayEffect> EquipmentEffect = NULL;
    FGameplayTag SlotTag = ArmorEquipped.EquipmentSlot;
    if (SlotTag == TAG_Equipment_Armor_Helmet)
    {
        EquipmentEffect = HelmetEffect;
    }
    else if (SlotTag == TAG_Equipment_Armor_Chestplate)
    {
        EquipmentEffect = ChestPlateEffect;
    }

    if (!IsValid(EquipmentEffect))
    {
        UE_LOG(LogTemp, Error, TEXT("No effect for an equipment effect"));
        return;
    }
        
    FGameplayEffectSpecHandle SpecHandle = OwningASC->MakeOutgoingSpec(EquipmentEffect,0, OwningASC->MakeEffectContext());

    TMap<FGameplayTag, float> AccumulatedStats = TMap<FGameplayTag,float>();

    AccumulatedStats.Add(ArmorEquipped.MainStat, ArmorEquipped.MainStatValue);

    for(TMap<FGameplayTag,float>::TRangedForIterator Itr = ArmorEquipped.Substats.begin(); Itr != ArmorEquipped.Substats.end(); ++Itr)
    {
        FGameplayTag statTag = Itr->Key;
        float statValue = Itr->Value;
        float *accumulatedValue = AccumulatedStats.Find(statTag);

        if (accumulatedValue != nullptr)
        {
            AccumulatedStats.Add(statTag, statValue + *accumulatedValue);
        }
        else
        {
            AccumulatedStats.Add(statTag, statValue);
        }
    }

    if (AccumulatedStats.Num() == 0)
    {
        return;
    }

    int i = 0;
    for (TMap<FGameplayTag, float>::TRangedForIterator Itr2 = AccumulatedStats.begin(); Itr2 != AccumulatedStats.end(); ++Itr2)
    {
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Itr2->Key, Itr2->Value);
        i++;
        if (i == 5)
        {
            UE_LOG(LogTemp, Error, TEXT("Infinite loop detected!"));
            break;
        }
    }

    OwningASC->ApplyGameplayEffectSpecToSelf(*(SpecHandle.Data));

}

bool UEquipmentComponent::EquipArmorInSlot(FArmorData ArmorToWear,FArmorData& OccupiedArmor)
{
    FGameplayTag SlotToSwap = ArmorToWear.EquipmentSlot;

    if (SlotToSwap == FGameplayTag::EmptyTag)
        return false;

    OccupiedArmor = UnEquipArmor(SlotToSwap);
    TurnArmorIntoStats(ArmorToWear);

    EquippedArmor.Add(SlotToSwap, ArmorToWear);

    return true;
}

FArmorData UEquipmentComponent::UnEquipArmor(FGameplayTag SlotToUnequip)
{
    FArmorData* UnequippedArmor = EquippedArmor.Find(SlotToUnequip);

    if (UnequippedArmor != nullptr)
    {
        EquippedArmor.Remove(SlotToUnequip);
        OwningASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(SlotToUnequip));

        return *UnequippedArmor;
    }

    return FArmorData();
}

FArmorData UEquipmentComponent::GetArmorDataFromSlot(FGameplayTag EquipmentSlot) const
{
    return EquippedArmor.Find(EquipmentSlot) ? EquippedArmor[EquipmentSlot] : FArmorData();
}
