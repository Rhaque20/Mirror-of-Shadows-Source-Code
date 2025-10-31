// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemySpawnerSystem.h"
#include "GAS/UDGameplayTags.h"
#include "EnemyCharacterBase.h"
#include "EnemyGroupBehaviorManager.h"
#include "Systems/ItemChest.h"
#include "Systems/InvisibleBarrierSystem.h"


// Sets default values
AEnemySpawnerSystem::AEnemySpawnerSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    _amtOfEnemies = 4;
    _batchItr = 0;
    _nmeItr = 0;
    enemyInstList.Reserve(_amtOfEnemies);
    spawnLocations.Reserve(batchSize);
}

// Called when the game starts or when spawned
void AEnemySpawnerSystem::BeginPlay()
{
    GrouperSetup();

    for(AItemChest* ItemChests : ItemChestsOwned)
    {
        OnAllEnemiesDead.AddDynamic(ItemChests, &AItemChest::UnlockChest);
        ItemChests->LockChest();
    }

    for (int i = 0; FlyingSpots.Num() > i; ++i)
    {
        FlyingSpots[i] += GetActorLocation();
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GroupManager = GetWorld()->SpawnActor<AEnemyGroupBehaviorManager>(GroupBehaviorBPRef, GetActorLocation(), GetActorRotation(), SpawnParams);
    
    if (!InvisibleBarrierSystem && InvisibleBarrierClass != nullptr)
    {
        InvisibleBarrierSystem = GetWorld()->SpawnActor<AInvisibleBarrierSystem>(InvisibleBarrierClass, GetActorLocation(),GetActorRotation(),SpawnParams);
    }

    if (InvisibleBarrierSystem)
    {
        GroupManager->OnBattleBegin.AddDynamic(InvisibleBarrierSystem,&AInvisibleBarrierSystem::BarriersOn);
        OnAllEnemiesDead.AddDynamic(InvisibleBarrierSystem,&AInvisibleBarrierSystem::BarriersOff);
    }

    switch (spawningLevel)
    {
    case EEnemyDifficulty::Normal_Mode:
        // Normal logic
        SpawnEnemyBatch();
        break;
    case EEnemyDifficulty::Hard_Mode:
        // Hard logic
        SpawnEnemyBatch();
        break;
    case EEnemyDifficulty::Impossible_Mode:
        // Debug logic
        SpawnAllEnemies();
        break;
    }

	Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("C++ BeginPlay"));

	//for(TMap<FString, AEnemyCharacterBase*>::TRangedForIterator Itr = enemyClassMap.begin(); Itr != enemyClassMap.end(); ++Itr)
 //   {
 //       //UE_LOG(Itr->Key + ": Enemy Class Here\n");
 //       Itr->Value->GetClass()->GetName();
 //       UE_LOG(LogTemp, Display, TEXT("%s: Enemy Class Here\n"), static_cast<FString>(Itr->Key));
 //   }
}

// Called every frame
void AEnemySpawnerSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/// @brief Test function set to spawn all enemies into the arena at once. Let batch size = num of enemy instances in the Unreal Editor for simplest tests
void AEnemySpawnerSystem::SpawnAllEnemies()
{
    int32 numOfBatches = 1;
    if(batchSize >= enemyInstList.Num())
    {
        batchSize = enemyInstList.Num();
        _usedSpawnLocals.Reserve(batchSize);
    }
    else
    {
        numOfBatches = enemyInstList.Num() / batchSize;
        numOfBatches += enemyInstList.Num() % batchSize;
    }

    for(_batchItr = 0; _batchItr < numOfBatches; ++_batchItr)
    {
        SpawnEnemyBatch();
    }
}

/// @brief - Main function that spawns an enemy in a random location
/// @param idx - The random spawn location chosen for each ordered enemy type.
/// @return - Shows if the enemy was spawned or not.
bool AEnemySpawnerSystem::SpawnEnemy(int32 idx)
{
    if(RandomSpawnLocation() >= 0)
    {
        if (!_usedSpawnLocals.IsValidIndex(idx))
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid index for spawning an enemy: %d"), idx);
            return false;
        }
        FVector SpawnLocation = spawnLocations[_usedSpawnLocals[idx]].GetLocation() + GetActorLocation(); // Example: staggered spawn
        FRotator SpawnRotation = FRotator::ZeroRotator;

        SpawnEnemyByIndex(idx, SpawnLocation, SpawnRotation);
        return true;
    }
    UE_LOG(LogTemp, Error, TEXT("Enemy failed to be spawned in a random location: %d"), idx);
    return false;
}

/// @brief Simple function that auto spawns the next batch of enemies at random choice of preset locations from your chosen enemy list
void AEnemySpawnerSystem::SpawnEnemyBatch()
{
    for(int32 j = 0; j < spawnLocations.Num() && (j + (batchSize * _batchItr)) < enemyInstList.Num(); ++j)
    {
        while(SpawnEnemy(j) == false)
        {
            UE_LOG(LogTemp, Error, TEXT("Double-check any instances of the SpawnEnemy(int32) function"));
        }
        ++_nmeItr;
    }
    ++_batchItr;
}

void AEnemySpawnerSystem::GrouperSetup()
{
    enemyNames.Reserve(enemyInstList.Num());
    _usedSpawnLocals.Reserve(batchSize);
    _amtOfEnemies = 0;
    for (int32 i = 0; i < enemyInstList.Max(); ++i)
    {
        _amtOfEnemies += 1;
    }
}

/// @brief Returns a random unused spawn location within this current batch. The value will also be added to the _usedSpawnLocals array if valid.
/// @return spawnLocations index - use this return for verification purposes; -1 will be used for failed gens
int32 AEnemySpawnerSystem::RandomSpawnLocation()
{
    int32 randNum = FMath::RandRange(0, batchSize-1);
    if (_usedSpawnLocals.Contains(randNum))
        return -1;
    
    if (_usedSpawnLocals.Num() < batchSize)
    {
        _usedSpawnLocals.Add(randNum);
        return randNum;
    }
    else if(_usedSpawnLocals.Contains(-1))
    {
        int32 deadIdx = _usedSpawnLocals.Find(-1);
        _usedSpawnLocals.RemoveAt(deadIdx);
        _usedSpawnLocals.Insert(randNum, deadIdx);
        return randNum;
    }
    else return -1;
}

bool AEnemySpawnerSystem::IsEnemyDead()
{
    return enemyNames.Contains("") && _usedSpawnLocals.Contains(-1);
}

bool AEnemySpawnerSystem::IsEnemyBatchDead()
{
    return _enemyClassMap.IsEmpty();
}

void AEnemySpawnerSystem::HandleEnemyDeath(ARPGCharacterBase* charBase)
{
    OnEnemyDeath(charBase);
    _enemyDeaths++;
    UE_LOG(LogTemp,Display,TEXT("Incremented enemy death"));
    switch (spawningLevel)
    {
    case EEnemyDifficulty::Normal_Mode:
        // Normal logic
        if(IsEnemyBatchDead())
        {
            _usedSpawnLocals.Empty();
            enemyNames.Empty();
            SpawnEnemyBatch();
            UE_LOG(LogTemp, Display, TEXT("Normal mode enemies have spawned in."));
        }
        break;
    case EEnemyDifficulty::Hard_Mode:
        // Hard logic
        if(IsEnemyDead())
        {
            int32 oldSpwIdx = _usedSpawnLocals.Find(-1);
            if(_nmeItr < enemyInstList.Num())
            {
                while (SpawnEnemy(oldSpwIdx) == false)
                {
                    UE_LOG(LogTemp, Display, TEXT("Double-check any instances of the SpawnEnemy(int32) function"));
                }
            }
            
            UE_LOG(LogTemp, Display, TEXT("Hard mode enemy has spawned in."));
            ++_nmeItr;
        }
        break;
    case EEnemyDifficulty::Impossible_Mode:
        // Debug logic
        break;
    }

    if (_enemyDeaths == enemyInstList.Num())
    {
        UE_LOG(LogTemp, Display, TEXT("ALL ENEMIES ARE DEAD! Just dead."));
        OnAllEnemiesDead.Broadcast(this);
    }
}

void AEnemySpawnerSystem::OnEnemyDeath(ARPGCharacterBase* charBase)
{
    //UE5's safe version of casting: Cast<> - acts like this: charBase && charBase->IsA(AEnemyCharacterBase::StaticClass()) ? static_cast<AEnemyCharacterBase*>(charBase) : nullptr;
    AEnemyCharacterBase* dyingFoe = Cast<AEnemyCharacterBase>(charBase);
    if(dyingFoe == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("RPGCharacter Enemy failed to be cast correctly."));
        return;
    }
    int32 enemyIdx = dyingFoe->SpawnPortalIdx;
    if (enemyIdx >= _usedSpawnLocals.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid index for checking used spawn locations upon enemy death %d"), enemyIdx);
        return;
    }

    AEnemyCharacterBase** deadEnemy = _enemyClassMap.Find(enemyNames[enemyIdx]);
    if (deadEnemy != nullptr)
    {
        _enemyClassMap.Remove(enemyNames[enemyIdx]);
        _usedSpawnLocals[enemyIdx] = -1;
        enemyNames[enemyIdx] = "";
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("The grouper system failed to kill enemy reference with the name %s"), *enemyNames[enemyIdx]);
        return;
    }
}

AEnemyCharacterBase* AEnemySpawnerSystem::SpawnEnemyByIndex(int32 BatchIndex, const FVector& Location, const FRotator& Rotation)
{
    if(BatchIndex >= batchSize || !_usedSpawnLocals.IsValidIndex(BatchIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid index for checking used spawn locations %d"), BatchIndex);
        return nullptr;
    }
    //int32 typeIdx = BatchIndex + (batchSize * _batchItr);
    FString local("_Locator");

    if (!enemyInstList.IsValidIndex(_nmeItr) || _nmeItr > _amtOfEnemies)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid index for spawning enemies %d"), _nmeItr);
        return nullptr;
    }

    TSubclassOf<AEnemyCharacterBase> EnemyClass = enemyInstList[_nmeItr];
    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Enemy class is null at index %d"), _nmeItr);
        return nullptr;
    }

    UWorld* World = GetWorld();
    if (!World) return nullptr;

    FString EnemyName;
    if(_usedSpawnLocals[BatchIndex] >= 10 && _usedSpawnLocals[BatchIndex] < 100)
        EnemyName = EnemyClass->GetName() + local + "0" + FString::FromInt(_usedSpawnLocals[BatchIndex]);
    else EnemyName = EnemyClass->GetName() + local + FString::FromInt(_usedSpawnLocals[BatchIndex]);

    if(enemyNames.Contains(""))
    {
        int32 deadIdx = enemyNames.Find("");
        enemyNames.RemoveAt(deadIdx);
        enemyNames.Insert(EnemyName, deadIdx);
    }
    else enemyNames.Add(EnemyName);
    //FCString::Atoi(*EnemyName);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AEnemyCharacterBase* SpawnedEnemy = World->SpawnActor<AEnemyCharacterBase>(EnemyClass, Location, Rotation, SpawnParams);
    if (SpawnedEnemy)
    {
        _enemyClassMap.Add(EnemyName, SpawnedEnemy);
        GroupManager->ReceiveEnemy(SpawnedEnemy);
        SpawnedEnemy->CharacterDied.AddDynamic(this, &AEnemySpawnerSystem::HandleEnemyDeath);
        SpawnedEnemy->SpawnPortalIdx = BatchIndex;
        UE_LOG(LogTemp, Display, TEXT("Spawned Enemy: %s"), *EnemyName);
        if (SpawnedEnemy->ActorHasTag("Flying"))
            SpawnedEnemy->GiveFlyingPositions(FlyingSpots);
    }

    return SpawnedEnemy;
}

