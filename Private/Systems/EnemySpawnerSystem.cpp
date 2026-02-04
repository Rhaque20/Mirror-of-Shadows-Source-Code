// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Systems/EnemySpawnerSystem.h"
#include "GAS/UDGameplayTags.h"
#include "EnemyCharacterBase.h"
#include "Systems/EnemyGroupBehaviorManager.h"
#include "Systems/ItemChest.h"
#include "Systems/InvisibleBarrierSystem.h"
#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif



// Sets default values
AEnemySpawnerSystem::AEnemySpawnerSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    _amtOfEnemies = 4;
    _batchItr = 0;
    _nmeItr = 0;
    enemyInstList.Reserve(_amtOfEnemies);
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    
    #if WITH_EDITOR
        PrimaryActorTick.bCanEverTick = true;
        PrimaryActorTick.bStartWithTickEnabled = true;
        bRunConstructionScriptOnDrag = true;
    #else
        PrimaryActorTick.bCanEverTick = false;
    #endif
}

// Called when the game starts or when spawned
void AEnemySpawnerSystem::BeginPlay()
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    if (!InvisibleBarrierSystem && InvisibleBarrierClass != nullptr)
    {
        InvisibleBarrierSystem = GetWorld()->SpawnActor<AInvisibleBarrierSystem>(InvisibleBarrierClass, GetActorLocation(),GetActorRotation(),SpawnParams);
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
#if WITH_EDITOR
void AEnemySpawnerSystem::DrawGizmos()
{
    int m = EnemyGroups.Num();
    for (int32 i = 0; i < m; ++i)
    {
        if (!EnemyGroups[i].bIsGroupVisible)
            continue;
        
        int n = EnemyGroups[i].EnemyGroupArray.Num();
        for (int32 j = 0; j < n; ++j)
        {
            const FEnemyEntryData& Group = EnemyGroups[i].EnemyGroupArray[j];
            FVector Location = GetActorTransform().TransformPosition(Group.SpawnTransform.GetLocation());
            DrawDebugSphere(
            GetWorld(),
            Location,
            GizmoSphereRadius,
            8,
            EnemyGroups[i].GizmoColor,
            false,
            -1.f,
            0,
            1.f
            );
            
            if (GEngine && GetWorld())
            {
                DrawDebugString(
                    GetWorld(),
                    FVector::ZeroVector + FVector(0,0,200),
                    TEXT("DEBUG STRING TEST"),
                    nullptr,
                    FColor::Green,
                    0.f,
                    true
                );
            }

            
            
        }
    }
}
void AEnemySpawnerSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    

    if (IsSelected())
        DrawGizmos();
    
}
#endif


/// @brief Simple function that auto spawns the next batch of enemies at random choice of preset locations from your chosen enemy list
bool AEnemySpawnerSystem::SpawnEnemyBatch()
{
    if (IsDefeated == false)
    {
        if (EnemyGroups.IsEmpty())
            return false;
        
        CurrentBatch = EnemyGroups[0].EnemyGroupArray;
        EnemyGroups.RemoveAt(0);
        for (FEnemyEntryData entry : CurrentBatch)
        {
            SpawnEnemyByEntry(entry);
        }
        
        return true;
    }
    
    return false;
}

void AEnemySpawnerSystem::GrouperSetup()
{
    
}

bool AEnemySpawnerSystem::IsEnemyBatchDead()
{
    return _activeEnemies.IsEmpty();
}

void AEnemySpawnerSystem::HandleEnemyDeath(ARPGCharacterBase* charBase)
{
    OnEnemyDeath(charBase);
    _enemyDeaths++;
    UE_LOG(LogTemp,Display,TEXT("Incremented enemy death"));
    
    if(IsEnemyBatchDead())
    {
        if (!SpawnEnemyBatch())
        {
            UE_LOG(LogTemp, Display, TEXT("ALL ENEMIES ARE DEAD! Just dead."));
            OnAllEnemiesDead.Broadcast(this);
        }
        else
            UE_LOG(LogTemp, Display, TEXT("Normal mode enemies have spawned in."));
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
    // int32 enemyIdx = dyingFoe->SpawnPortalIdx;
    // if (enemyIdx >= _usedSpawnLocals.Num())
    // {
    //     UE_LOG(LogTemp, Error, TEXT("Invalid index for checking used spawn locations upon enemy death %d"), enemyIdx);
    //     return;
    // }

    AEnemyCharacterBase** deadEnemy = _activeEnemies.Find(dyingFoe);
    if (deadEnemy != nullptr)
    {
        _activeEnemies.Remove(dyingFoe);
        // _usedSpawnLocals[enemyIdx] = -1;
        // enemyNames[enemyIdx] = "";
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("The grouper system failed to kill enemy reference with the name %s"), *dyingFoe->GetName());
        return;
    }
}

void AEnemySpawnerSystem::ActivateSpawner(int MeleeTickets, int RangedTickets)
{
    for (int i = 0; FlyingSpots.Num() > i; ++i)
    {
        FlyingSpots[i] += GetActorLocation();
    }
    
    GrouperSetup();

    for(AItemChest* ItemChests : ItemChestsOwned)
    {
        OnAllEnemiesDead.AddDynamic(ItemChests, &AItemChest::UnlockChest);
        ItemChests->LockChest();
    }

    if (GroupManager == nullptr)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        GroupManager = GetWorld()->SpawnActor<AEnemyGroupBehaviorManager>(GroupBehaviorBPRef, GetActorLocation(), GetActorRotation(), SpawnParams);
    }
    
    if (InvisibleBarrierSystem)
    {
        GroupManager->OnBattleBegin.AddDynamic(InvisibleBarrierSystem,&AInvisibleBarrierSystem::BarriersOn);
        OnAllEnemiesDead.AddDynamic(InvisibleBarrierSystem,&AInvisibleBarrierSystem::BarriersOff);
    }
    
    
    SpawnEnemyBatch();

    /*switch (spawningLevel)
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
    }*/
}

AEnemyCharacterBase* AEnemySpawnerSystem::SpawnEnemyByEntry(FEnemyEntryData EnemyEntry)
{
    // if(BatchIndex >= batchSize || !_usedSpawnLocals.IsValidIndex(BatchIndex))
    // {
    //     UE_LOG(LogTemp, Error, TEXT("Invalid index for checking used spawn locations %d"), BatchIndex);
    //     return nullptr;
    // }
    // int32 typeIdx = BatchIndex + (batchSize * _batchItr);
    // FString local("_Locator");
    //
    // if (!enemyInstList.IsValidIndex(_nmeItr) || _nmeItr > _amtOfEnemies)
    // {
    //     UE_LOG(LogTemp, Error, TEXT("Invalid index for spawning enemies %d"), _nmeItr);
    //     return nullptr;
    // }

    TSubclassOf<AEnemyCharacterBase> EnemyClass = EnemyEntry.EnemyClass;
    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Enemy class is null at index %d"), _nmeItr);
        return nullptr;
    }

    UWorld* World = GetWorld();
    if (!World) return nullptr;

    // FString EnemyName;
    // // if(_usedSpawnLocals[BatchIndex] >= 10 && _usedSpawnLocals[BatchIndex] < 100)
    // //     EnemyName = EnemyClass->GetName() + local + "0" + FString::FromInt(_usedSpawnLocals[BatchIndex]);
    // // else EnemyName = EnemyClass->GetName() + local + FString::FromInt(_usedSpawnLocals[BatchIndex]);
    //
    // EnemyName = EnemyClass->GetName() + local + "0" + FString::FromInt(typeIdx);

    // if(enemyNames.Contains(""))
    // {
    //     int32 deadIdx = enemyNames.Find("");
    //     enemyNames.RemoveAt(deadIdx);
    //     enemyNames.Insert(EnemyName, deadIdx);
    // }
    // else enemyNames.Add(EnemyName);
    //FCString::Atoi(*EnemyName);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AEnemyCharacterBase* SpawnedEnemy = World->SpawnActor<AEnemyCharacterBase>(EnemyClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
    if (SpawnedEnemy)
    {
        // _activeEnemies.Add(EnemyName, SpawnedEnemy);
        SpawnedEnemy->SetActorLocation(GetActorTransform().TransformPosition(EnemyEntry.SpawnTransform.GetLocation()));
        _activeEnemies.Add(SpawnedEnemy);
        GroupManager->ReceiveEnemy(SpawnedEnemy);
        SpawnedEnemy->CharacterDied.AddDynamic(this, &AEnemySpawnerSystem::HandleEnemyDeath);
        UE_LOG(LogTemp, Display, TEXT("Spawned Enemy: %s"), *SpawnedEnemy->GetName());
        
        if (SpawnedEnemy->ActorHasTag("Flying"))
            SpawnedEnemy->GiveFlyingPositions(FlyingSpots);
    }

    return SpawnedEnemy;
}

