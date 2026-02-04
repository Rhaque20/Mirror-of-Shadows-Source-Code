#pragma once
#include "CoreMinimal.h"
#include "EffectTargetType.generated.h"

UENUM(BlueprintType)
enum class EEffectTargetType : uint8
{
	OnFirstHit UMETA(DisplayName = "On First Hit", ToolTip = "Apply this effect on the first connected hit of the target"),
	OnHit UMETA(DisplayName = "On hit", ToolTip = "Apply this effect on every hit"),
	OnlySkill UMETA(DisplayName = "Only Skill", ToolTip = "Apply this effect for the duration of the skill and remove it after the skill ends"),
	OnCaster UMETA(DisplayName = "On Caster", ToolTip = "Applied to just the caster of the skill"),
	OnTeam UMETA(DisplayName = "On Team", ToolTip = "Applied to entire player team or for enemies")
};