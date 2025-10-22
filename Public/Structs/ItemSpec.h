#include "InstancedStruct.h"
#include <GameplayTagContainer.h>
#include "GAS/UDGameplayTags.h"
#include "Enumerator/ItemEnum.h"
#include "Structs/RPGItemStruct.h"
#include "Enumerator/ItemCategoryEnum.h"

#include "ItemSpec.generated.h"
#pragma once

UENUM(BlueprintType)
enum class EFragmentID : uint8
{
    Substat UMETA(DisplayName = "Substat"),
    MainStat UMETA(DisplayName = "MainStat"),
    ItemGrade UMETA(DisplayName = "Item Grade"),
    Upgrade UMETA(DisplayName = "Upgrade"),
    ArmorSlotandSet UMETA(DisplayName = "Armor Slot and Set"),
};

USTRUCT(BlueprintType)
struct FItemSpec
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Definition")
	class URPGItem* ItemDefinition;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Definition")
    bool bCanStack = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (EditCondition = "bCanStack"))
	int Stacks = 0;
    // Holds the mutable properties of the item for custom items.
    // UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ToolTip="Contains mutable properties of the item such as substats, equipment sets, etc."))
    // TArray<TInstancedStruct<FItemSpecFragments>> SpecFragments;
    TInstancedStruct<FRPGItemData> ItemData;

    EItemCategory ItemCategory;

    FName ItemName;

    template <std::derived_from<FRPGItemData> T>
    [[nodiscard]] const T* RetrieveItem() const
    {
        const TInstancedStruct<FRPGItemData>* Found = FindFragment_Internal(*T::StaticStruct());
        if (Found == nullptr)
            return nullptr;
        check(Found->IsValid());
        return Found->GetPtr<T>();
    }
    // Add an item spec fragment to this item based on the accepted struct types.
    template <std::derived_from<FRPGItemData> T>
    void SetInventoryItem(T& StructToUse)
    {
        ItemData = TInstancedStruct<FRPGItemData>::Make<T>(StructToUse);
    }
    // Don't use this one, it's for your K2 node later
    [[nodiscard]] const TInstancedStruct<FRPGItemData>* FindFragment_Internal(const UScriptStruct& Type) const
    {

        // Getting the script struct of the fragment, see if the fragment is NOT a child of the type passed in
        if (!ItemData.IsValid() || !ItemData.GetScriptStruct()->IsChildOf(&Type))
        {
            UE_LOG(LogTemp,Display,TEXT("Fragment is %s and trying to find %s"),*(ItemData.GetScriptStruct()->GetName()),*Type.GetName());
            return nullptr;
        }
                    
        return &ItemData;
        
    }


// Main function, use this
    // [[nodiscard]] means to return a compiler warning if the return value is not used after being called
    // template <std::derived_from<FItemSpecFragments> T>
    // [[nodiscard]] const T* FindFragment() const
    // {
    //     const TInstancedStruct<FItemSpecFragments>* Found = FindFragment_Internal(*T::StaticStruct());
    //     if (Found == nullptr)
    //         return nullptr;
    //     check(Found->IsValid());
    //     return Found->GetPtr<T>();
    // }
    //
    // // Add an item spec fragment to this item based on the accepted struct types.
    // template <std::derived_from<FItemSpecFragments> T>
    // void AddFragment(T& StructToUse)
    // {
    //     SpecFragments.Add(TInstancedStruct<FItemSpecFragments>::Make<T>(StructToUse));
    // }
    //
    // private: 
    //     // Don't use this one, it's for your K2 node later
    //     [[nodiscard]] const TInstancedStruct<FItemSpecFragments>* FindFragment_Internal(const UScriptStruct& Type) const
    //     {
    //         for (const TInstancedStruct<FItemSpecFragments>& S : SpecFragments)
    //         {
    //             if (!S.IsValid())
    //                 continue;
    //             // Getting the script struct of the fragment, see if the fragment is NOT a child of the type passed in
    //             if (!S.GetScriptStruct()->IsChildOf(&Type))
    //             {
    //                 UE_LOG(LogTemp,Display,TEXT("Fragment is %s and trying to find %s"),*(S.GetScriptStruct()->GetName()),*Type.GetName());
    //                 continue;
    //             }
    //                 
    //             return &S;
    //         }
    //         return nullptr;
    //     }
};

// USTRUCT(BlueprintType)
// struct FItemSpecFragments
// {
//     GENERATED_BODY()
//     FItemSpecFragments() {}
//
//     // Copy constructor and operator overload.
//     FItemSpecFragments(const FItemSpecFragments&) = default;
//     FItemSpecFragments& operator = (const FItemSpecFragments&) = default;
//
//     // Move constructor and operator overload.
//     FItemSpecFragments(FItemSpecFragments&&) = default;
//     FItemSpecFragments& operator = (FItemSpecFragments&&) = default;
//
//     // Virtual destructor requires us to declare copy, member wise, and move constructors.
//     virtual ~FItemSpecFragments() {}
//     
// };
//
// USTRUCT(BlueprintType)
// struct FStatValue
// {
//     GENERATED_BODY();
//
//     FStatValue()
//     {
//         
//     }
//
//     FStatValue(FGameplayTag tag, float value, int rarity)
//     {
//         StatTag = tag;
//         StatValue = value;
//         StatRarity = rarity;
//     }
//
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     FGameplayTag StatTag;
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     float StatValue;
//
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     int StatRarity;
//
// };
//
// USTRUCT(BlueprintType)
// struct FEquipmentSubstats : public FItemSpecFragments
// {
//     GENERATED_BODY();
//
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     TArray<FStatValue> Substats;
// };
//
// USTRUCT(BlueprintType)
// struct FEquipmentMainStat : public FItemSpecFragments
// {
//     GENERATED_BODY();
//
//     FEquipmentMainStat()
//     {
//         
//     }
//
//     FEquipmentMainStat(FGameplayTag tag, float value, int rarity)
//     {
//         MainStat = FStatValue(tag, value, rarity);
//     }
//
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     FStatValue MainStat;
// };
//
// USTRUCT(BlueprintType)
// struct FItemGrade : public FItemSpecFragments
// {
//     GENERATED_BODY();
//
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     EGrade ItemGrade;
// };
//
// USTRUCT(BlueprintType)
// struct FUpgrade : public FItemSpecFragments
// {
//     GENERATED_BODY();
//
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     int Level;
//
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     float EXP;
// };
//
// USTRUCT(BlueprintType)
// struct FArmorSlotandSet : public FItemSpecFragments
// {
//     GENERATED_BODY();
//
//     FArmorSlotandSet()
//     {
//         
//     }
//
//     FArmorSlotandSet(FGameplayTag SetTag, FGameplayTag SlotTag)
//     {
//         ArmorSlot = SlotTag;
//         ArmorSet = SetTag;
//     }
//
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     FGameplayTag ArmorSlot;
//
//     UPROPERTY(BlueprintReadWrite, EditAnywhere)
//     FGameplayTag ArmorSet;
// };