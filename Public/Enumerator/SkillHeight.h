#pragma once

#include "CoreMinimal.h"
#include "SkillHeight.generated.h"

UENUM(BlueprintType)
enum class ESkillHeight : uint8
{
    Grounded UMETA(DisplayName = "Grounded"),
    Aerial UMETA(DisplayName = "Aerial"),
    GroundorAir UMETA(DisplayName = "GroundorAir")
};