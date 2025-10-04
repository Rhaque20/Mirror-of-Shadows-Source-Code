#pragma once

#include "CoreMinimal.h"
#include "HitTypeEnum.generated.h"

UENUM(BlueprintType)
enum class EHitType : uint8
{
    Hit UMETA(DisplayName = "Hit"),
    CriticalHit UMETA(DisplayName = "Critical Hit"),
    Miss UMETA(DisplayName = "Miss")
};