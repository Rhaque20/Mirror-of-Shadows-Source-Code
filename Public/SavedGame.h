// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Structs/StructToSave.h"


#include "SavedGame.generated.h"


UCLASS()
class MIRROROFSHADOWS_API USavedGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, SaveGame)
    FInventoryToSave Inventory;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    FDungeonToSave DungeonManager;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TArray<FEquipmentToSave> EquipmentItems;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    FVector CharacterPosition;
	
	// Different from RecipesHeld in Inventory in that these are the redeemed recipes
	// Since RecipesHeld aren't immediately unlocked until the player enters a save station (for consumables and snacks)
	// Or at the hub (Equipment Recipes)
	UPROPERTY(BlueprintReadWrite, SaveGame)
	TSet<FGameplayTag> UnlockedRecipes;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<FName,FSkillTreeSaveData> SkillTreeData;
};
