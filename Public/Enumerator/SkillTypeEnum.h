#pragma once

#include "CoreMinimal.h"
#include "SkillTypeEnum.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	ComboATK UMETA(DisplayName = "Combo Attack"),
	HeavyATK UMETA(DisplayName= "Heavy Attack"),
	SkillATK UMETA(DisplayName="Skill Attack")
};