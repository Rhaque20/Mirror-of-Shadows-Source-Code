// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/DungeonManager.h"
#include "GAS/UDGameplayTags.h"
#include "EnemySpawnerSystem.h"
#include "PlayerPartyController.h"
#include "Blueprint/UserWidget.h"
#include "Systems/ItemChest.h"

// Sets default values
ADungeonManager::ADungeonManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADungeonManager::BeginPlay()
{
	Super::BeginPlay();

	bItemChestClaimed.SetNum(ItemChestsReferences.Num());
	bEnemyGroupsCleared.SetNum(EnemyGroupReferences.Num());

	for (int i = 0; i < ItemChestsReferences.Num(); i++)
	{
		ItemChestsReferences[i]->ChestToggle(!bItemChestClaimed[i]);
		ItemChestsReferences[i]->OnChestOpen.AddDynamic(this,&ADungeonManager::OnChestClaimed);
		ItemChestsReferences[i]->GenerateArmorsToGive(ItemChestsReferences[i]->IsRareChest() ? Tier1LootGrade : Tier2LootGrade,SetTag,FVector2D(1,3));
	}

	for (int i = 0; i < EnemyGroupReferences.Num(); i++)
	{
		
		if (EnemyGroupReferences[i]->ActorHasTag(BossTag))
		{
			EnemyGroupReferences[i]->OnAllEnemiesDead.AddDynamic(this,&ADungeonManager::OnDungeonClear);
		}
		else
		{
			EnemyGroupReferences[i]->OnAllEnemiesDead.AddDynamic(this,&ADungeonManager::OnSpawnerCleared);
		}
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController != nullptr)
	{
		Cast<APlayerPartyController>(PlayerController)->GameOver.AddDynamic(this,&ADungeonManager::OnDungeonFail);
	}
	
}

void ADungeonManager::OnDungeonClear(AEnemySpawnerSystem* DefeatedSpawner)
{
	UE_LOG(LogTemp, Display, TEXT("YOU WIN!"));
	OnVictory.Broadcast();
}

void ADungeonManager::OnSpawnerCleared(class AEnemySpawnerSystem* DefeatedSpawner)
{
	int index = EnemyGroupReferences.Find(DefeatedSpawner);

	if (index != -1)
	{
		bEnemyGroupsCleared[index] = true;
	}
}

void ADungeonManager::OnDungeonFail()
{
	if (GameOverWidget)
	{
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();
		CreateWidget(GetWorld(),GameOverWidget)->AddToViewport();
	}
}

void ADungeonManager::OnChestClaimed(AItemChest* ItemChest)
{
	int index = ItemChestsReferences.Find(ItemChest);

	if (index != -1)
	{
		bItemChestClaimed[index] = true;
	}
}

