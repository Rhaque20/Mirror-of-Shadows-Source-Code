// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include "GameFramework/Actor.h"
#include "Enumerator/GameDifficultyEnum.h"
#include "GameplayTagContainer.h"
#include "Structs/EnemyEntryStruct.h"

#include "EnemySpawnerSystem.generated.h"

class AEnemyCharacterBase;
class AEnemyGroupBehaviorManager;
using EnemyMap = TMap<FString, AEnemyCharacterBase*>;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllEnemiesDead,AEnemySpawnerSystem*,DefeatedSpawner);

UCLASS()
class MIRROROFSHADOWS_API AEnemySpawnerSystem : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AEnemySpawnerSystem();

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    bool SpawnEnemyBatch(); 

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    void GrouperSetup();

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    bool IsEnemyBatchDead();

    UFUNCTION()
    void HandleEnemyDeath(ARPGCharacterBase* charBase);

    UFUNCTION()
    void OnEnemyDeath(ARPGCharacterBase* charBase);

	void ActivateSpawner(int MeleeTickets, int RangedTickets);

    AEnemyCharacterBase* SpawnEnemyByEntry(FEnemyEntryData EnemyEntry);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Difficulty")
    EEnemyDifficulty spawningLevel;

    // Each enemy class in this list represents the order of enemies to be spawned into the level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Types and Location")
    TArray<TSubclassOf<AEnemyCharacterBase>> enemyInstList;

    UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
    TSubclassOf<AEnemyGroupBehaviorManager> GroupBehaviorBPRef;

    UPROPERTY(BlueprintAssignable,BlueprintCallable)
    FOnAllEnemiesDead OnAllEnemiesDead;

    UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
    TArray<class AItemChest*> ItemChestsOwned;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class AInvisibleBarrierSystem> InvisibleBarrierClass;

	UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
	AInvisibleBarrierSystem* InvisibleBarrierSystem;

	UPROPERTY(BlueprintReadOnly,EditInstanceOnly, Meta = (MakeEditWidget = true))
	TArray<FVector> FlyingSpots;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawnParams", meta=(ClampMin = 1, ClampMax = 10))
	int32 NumOfBatches = 1;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	#if WITH_EDITOR
		virtual bool ShouldTickIfViewportsOnly() const override { return true; }
		void DrawGizmos();
		virtual void Tick(float DeltaTime) override;
	#endif

public:	
    bool IsDefeated = false;

protected:
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	AEnemyGroupBehaviorManager* GroupManager;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Spawn System")
	TArray<FEnemyGroupData> EnemyGroups;
	
	UPROPERTY(EditAnywhere, Category="Spawn System")
	int32 SelectedGroupIndex = -1;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditInstanceOnly, Category = "Spawn System", meta =(ClampMin = 50.0f, ClampMax = 1000.0f))
	float GizmoSphereRadius = 50.0f;
#endif

private:
	UPROPERTY()
	TSet<AEnemyCharacterBase*> _activeEnemies;
    int32 _amtOfEnemies;
    int32 _batchItr;
    int32 _nmeItr;
	int32 _enemyDeaths = 0;
	
	TArray<FEnemyEntryData> CurrentBatch;
	


};
