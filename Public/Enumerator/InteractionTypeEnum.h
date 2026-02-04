#pragma once

#include "CoreMinimal.h"
#include "InteractionTypeEnum.generated.h"

UENUM(BlueprintType)
enum class EInteractiveType : uint8
{
	None UMETA(DisplayName = "None"),
	Normal UMETA(DisplayName = "Normal"),
	RespawnPostDungeon UMETA(DisplayName = "Respawn Post Dungeon"),
	OneTime UMETA(DisplayName = "One Time"),
	
};