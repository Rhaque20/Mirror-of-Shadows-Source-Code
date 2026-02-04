#pragma once

#include "CoreMinimal.h"
#include "FactionHitType.generated.h"

UENUM(BlueprintType)
enum class EFactionHitType : uint8
{
	None UMETA(DisplayName="None"),
	Damage UMETA(DisplayName = "Damage", ToolTip = "Deal damage or negative effects to this faction"),
	Support UMETA(DisplayName = "Support", ToolTip = "Heal or apply positive effects to this faction")
};