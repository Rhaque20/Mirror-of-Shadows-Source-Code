#pragma once
#include "CoreMinimal.h"
#include "EffectTargetType.generated.h"

UENUM(BlueprintType)
enum class EEffectTargetType : uint8
{
	OnFirstHit UMETA(DisplayName = "On First Hit", ToolTip = "Apply this effect on the first connected hit of the target"),
	OnHit UMETA(DisplayName = "On hit", ToolTip = "Apply this effect on every hit"),
	OnCaster UMETA(DisplayName = "On Caster", ToolTip = "Applied to just the caster of the skill"),
	OnTeam UMETA(DisplayName = "On Team", ToolTip = "Applied to entire player team or for enemies")
};