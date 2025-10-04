#pragma once

#include "CoreMinimal.h"
#include "AttackCategoryEnum.generated.h"

UENUM(BlueprintType)
enum class EAttackCategory : uint8
{
    NormalAttack UMETA(DisplayName = "Normal Attack"),
    AirAttack UMETA(DisplayName="Air Attack"),
    HeavyAttack UMETA(DisplayName="Heavy Attack"),
    SkillAttack UMETA(DisplayName = "Skill Attack")
    
};