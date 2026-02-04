// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/ItemSpec.h"
#include "Structs/ArmorDataStruct.h"
#include "DataAsset/SkillTreeNodeData.h"
#include "StructToSave.generated.h"


USTRUCT(BlueprintType)
struct FInventoryToSave
{   
    GENERATED_BODY()
     
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<FName,FItemSpec> ItemsToSave;
    
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSet<FGameplayTag> KeyItems;
    
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSet<FGameplayTag> RecipesHeld;
    
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<int,FArmorData> SavedArmor;

};

USTRUCT(BlueprintType)
struct FDungeonToSave
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSet<FGuid> SavedChests;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSet<FGuid> SavedEnemySpawner;
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSet<FGuid> ActivatedInteractables;
    
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSet<FGuid> RespawnableInteractables;
};

USTRUCT(BlueprintType)
struct FEquipmentToSave
{
    GENERATED_BODY()

    UPROPERTY()
    TMap<FGameplayTag, FArmorData> SavedEquippedArmor;

};

USTRUCT(BlueprintType)
struct FSkillTreeSaveData
{
    GENERATED_BODY()

    // Use soft object pointers for UDataAsset references
    UPROPERTY(SaveGame)
    TArray<TSoftObjectPtr<USkillTreeNodeData>> ActiveSkillTreeNodes;

    // FName serializes directly without issue
    UPROPERTY(SaveGame,BlueprintReadOnly)
    TArray<FName> ActiveSkillTreeNodeNames;
    
    //Other stuff
    UPROPERTY(SaveGame,BLueprintReadOnly)
    float CurrentXP;
    
    UPROPERTY(SaveGame,BlueprintReadOnly)
    float CurrentAP;
    
    UPROPERTY(SaveGame,BlueprintReadOnly)
    int Level;
};