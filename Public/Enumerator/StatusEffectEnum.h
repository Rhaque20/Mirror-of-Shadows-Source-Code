#pragma once

#include "CoreMinimal.h"
#include "StatusEffectEnum.generated.h"

UENUM(BlueprintType)
enum class EStatusEffect : uint8
{
    Daze UMETA(DisplayName = "Daze"),
    Bleed UMETA(DisplayName = "Bleed"),
    Burn UMETA(DisplayName = "Burn"),
    Silence UMETA(DisplayName = "Silence"),
    Slow UMETA(DisplayName = "Slow"),
    Poison UMETA(DisplayName = "Poison"),
    Paralyze UMETA(DisplayName= "Paralyze"),
    Blind UMETA(DisplayName ="Blind")
};