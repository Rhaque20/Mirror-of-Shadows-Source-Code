#pragma once

#include "CoreMinimal.h"
#include "GameDifficultyEnum.generated.h"

UENUM(BlueprintType)
enum class EEnemyDifficulty : uint8
{
    Normal_Mode UMETA(DisplayName = "Normal Mode"),
    Hard_Mode UMETA(DisplayName = "Hard Mode"),
    Impossible_Mode  UMETA(DisplayName = "Impossible Mode")
};