#pragma once

#include "CoreMinimal.h"
#include "EnumLib.generated.h"

UENUM(BlueprintType)
enum class EElement: uint8
{
	Physical UMETA(DisplayName = "Physical"),
    Fire UMETA(DisplayName = "Fire"),
    Wind UMETA(DisplayName = "Wind"),
    Earth UMETA(DisplayName = "Earth"),
    Water UMETA(DisplayName = "Water"),
    Light UMETA(DisplayName = "Light"),
    Dark UMETA(DisplayName = "Dark"),
    Pure UMETA(DisplayName = "Pure")
};

UENUM(BlueprintType)
enum class EStats: uint8
{
	Health UMETA(DisplayName = "Health"),
    Attack UMETA(DisplayName = "Attack"),
    Defense UMETA(DisplayName = "Defense"),
    Potency UMETA(DisplayName = "Potency"),
    Resistance UMETA(DisplayName = "Resistance"),
    CritRate UMETA(DisplayName = "Crit Rate"),
    CritDMG UMETA(DisplayName = "Crit DMG"),
    SPGain UMETA(DisplayName = "SP Gain")
};