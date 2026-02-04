// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enumerator/ItemEnum.h"
#include "GameFramework/Actor.h"
#include "Structs/StructToSave.h"
#include "Interfaces/ISavableInterface.h"
#include "DungeonManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictory);
UCLASS()
class MIRROROFSHADOWS_API ADungeonManager : public AActor, public ISavableInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ADungeonManager();

    UFUNCTION()
    FDungeonToSave GetSaveData();

    /*UFUNCTION()
    void LoadSavedData(FDungeonToSave& LoadedData);*/

    virtual void InitializeData() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnDungeonClear(class AEnemySpawnerSystem* DefeatedSpawner);

    UFUNCTION()
    void OnSpawnerCleared(class AEnemySpawnerSystem* DefeatedSpawner);

    UFUNCTION()
    void OnDungeonFail();

    UFUNCTION()
    void OnChestClaimed(class AItemChest* ItemChest);
    
    UFUNCTION()
    void OnUnlockingInteractable(class AInteractableActor* ItemChest);

protected:
    UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
    TArray<class AItemChest*> ItemChestsReferences;
    
    UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
    int32 MaxMeleeTickets = 1;
    
    UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
    int32 MaxRangedTickets = 1;

    UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
    TArray<class AEnemySpawnerSystem*> EnemyGroupReferences;
    
    UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
    TArray<class AInteractableActor*> LockedInteractables;
    
    // This is for save data purposes.
    UPROPERTY()
    TSet<FGuid> EnemyGroupsCleared;
    // This is for save data purposes.
    UPROPERTY()
    TSet<FGuid> ChestsClaimed;
    
    // This is for save data purposes.
    UPROPERTY()
    TSet<FGuid> UnlockedInteractables;

    UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
    class ACheckPointSystem* CheckPointSystem;

    UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Loot Parameters")
    TMap<EGrade, float> Tier1LootGrade;

    UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Loot Parameters")
    TMap<EGrade, float> Tier2LootGrade;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    FName BossTag = "Boss";

    UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Loot Parameters")
    FGameplayTag SetTag;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TSubclassOf<class UUserWidget> GameOverWidget;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TSubclassOf<class UUserWidget> VictoryWidget;

    //Save stuff

    virtual int32 GetSavePhase() const override;
    virtual void SaveData(USavedGame& Root) override;
    virtual void LoadData(const USavedGame& Root) override;

public:
    UPROPERTY(BlueprintAssignable)
    FOnVictory OnVictory;

    int32 PhaseNumber;

private:
    bool bLoadedStuff = false;
    
private:
    UFUNCTION()
    void BindPlayerControllerForGameOver(APlayerController* PlayerController);


};
