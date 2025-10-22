// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include "GameFramework/Actor.h"
#include "Enumerator/GameDifficultyEnum.h"
#include "GameplayTagContainer.h"

#include "EnemyGrouperSystem.generated.h"

class AEnemyCharacterBase;
class AEnemyGroupBehaviorManager;
using EnemyMap = TMap<FString, AEnemyCharacterBase*>;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemiesDead);


UCLASS()
class MIRROROFSHADOWS_API AEnemyGrouperSystem : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AEnemyGrouperSystem();

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    void SpawnAllEnemies();

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    bool SpawnEnemy(int32 idx);

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    void SpawnEnemyBatch(); 

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    void GrouperSetup();

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    int32 RandomSpawnLocation();

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    bool IsEnemyDead();

    UFUNCTION(BlueprintCallable, Category = "Enemy Spawning")
    bool IsEnemyBatchDead();

    UFUNCTION()
    void HandleEnemyDeath(ARPGCharacterBase* charBase);

    UFUNCTION()
    void OnEnemyDeath(ARPGCharacterBase* charBase);

    AEnemyCharacterBase* SpawnEnemyByIndex(int32 TypeIndex, const FVector& Location, const FRotator& Rotation);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Difficulty")
    EEnemyDifficulty spawningLevel;

    // Each enemy class in this list represents the order of enemies to be spawned into the level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Types and Location")
    TArray<TSubclassOf<AEnemyCharacterBase>> enemyInstList;

    // List of spawnLocation will allow us to randomize which enemies are spawned where in this map
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Types and Location", Meta = (MakeEditWidget = true))
    TArray<FTransform> spawnLocations;

    // Determines the number of spawnLocation elements in Blueprints
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Types and Location")
    int32 batchSize = 0;

    UPROPERTY(VisibleInstanceOnly, Category = "Enemy Types")
    TArray<FString> enemyNames;

    UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
    TSubclassOf<AEnemyGroupBehaviorManager> GroupBehaviorBPRef;

    UPROPERTY(BlueprintAssignable,BlueprintCallable)
    FOnAllEnemiesDead OnAllEnemiesDead;

    UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
    TArray<class AItemChest*> ItemChestsOwned;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    EnemyMap _enemyClassMap;
    TArray<int32> _usedSpawnLocals;
    int32 _amtOfEnemies;
    int32 _batchItr;
    int32 _nmeItr;
	int32 _enemyDeaths = 0;

    AEnemyGroupBehaviorManager* GroupManager;

};
