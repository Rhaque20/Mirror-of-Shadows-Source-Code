#pragma once

#include "CoreMinimal.h"
#include "ParryableHitTypeEnum.generated.h"

UENUM(BlueprintType)
enum class EParryableHitType : uint8
{
	NormalHit UMETA(DisplayName = "Normal Hit", ToolTip = "Used for generic hits"),
	UnBlockable UMETA(DisplayName = "Un Parryable", ToolTip = "Used for hits that are not blockable but dodgeable"),
	UnDodgeable UMETA(DisplayName = "Un Dodgeable", ToolTip = "Used for hits that are not dodgeable (and won't trigger perfect dodge) but deflectable"),
	UnDefendable UMETA(DisplayName = "UnDefendable", ToolTip = "Used for attacks that cannot be parried, blocked, or dodged")
};
