#pragma once

#include "CoreMinimal.h"
#include "ItemCategoryEnum.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
    None UMETA(DisplayName = "None"),
    CraftingMaterial UMETA(DisplayName = "Crafting Material"),
    Armor UMETA(DisplayName = "Armor"),
    Weapon UMETA(DisplayName ="Weapon"),
    CraftingRecipe UMETA(DisplayName = "Crafting Recipe"),
    Consumable UMETA(DisplayName = "Consumable"),
};