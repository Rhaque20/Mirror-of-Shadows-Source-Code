#include "Systems/DestructibleActor.h"

ADestructableActor::ADestructableActor()
{
	if (InteractiveType == EInteractiveType::OneTime || InteractiveType == EInteractiveType::RespawnPostDungeon)
	{
		DestructableID = FGuid::NewGuid();
	}
}

void ADestructableActor::BeginPlay()
{
	CurrentHP = MaxHP;
	Super::BeginPlay();
}
