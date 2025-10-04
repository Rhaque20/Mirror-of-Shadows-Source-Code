// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGrouperSystem.h"

// Sets default values
AEnemyGrouperSystem::AEnemyGrouperSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyGrouperSystem::BeginPlay()
{
	Super::BeginPlay();

    _enemyCounter.Reserve(numOfEnemiesPerType.Num());
    _amtOfEnemies = 0;
    for (int32 i = 0; i < numOfEnemiesPerType.Max(); ++i)
    {
        _amtOfEnemies += numOfEnemiesPerType[i];
    }
    enemyNames.Reserve(_amtOfEnemies);

    for (int32 i = 0; i < _enemyCounter.Max(); ++i)
    {
        _enemyCounter.Add(1);
    }

    for (int32 i = 0; i < _amtOfEnemies; ++i)
    {
        enemyNames.Add( FString() );
    }

    int32 i = 0, j = 0;
    TArray<int32> nmeNum;
    nmeNum.Reserve(_enemyCounter.Max());
    for(int32 k = 0; k < _enemyCounter.Max(); ++k)
    {
        nmeNum.Add(1);
    }

    for (TSubclassOf<AEnemyCharacterBase> className : enemyInstList)
    {
        UE_LOG(LogTemp, Display, TEXT("Enemy Class: %s\n"), static_cast<FString>(className->GetName()));
        while(nmeNum[j] <= numOfEnemiesPerType[j])
        {
            enemyNames[i] = className->GetName() + FString::FromInt(nmeNum[j]);
            nmeNum[j] += 1;
            ++i;
        }
        ++j;
    }

    if(spawnAllEnemiesAtOnce) SpawnAllEnemies();

	//for(TMap<FString, AEnemyCharacterBase*>::TRangedForIterator Itr = enemyClassMap.begin(); Itr != enemyClassMap.end(); ++Itr)
 //   {
 //       //UE_LOG(Itr->Key + ": Enemy Class Here\n");
 //       Itr->Value->GetClass()->GetName();
 //       UE_LOG(LogTemp, Display, TEXT("%s: Enemy Class Here\n"), static_cast<FString>(Itr->Key));
 //   }
}

// Called every frame
void AEnemyGrouperSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyGrouperSystem::SpawnAllEnemies()
{
    for (int32 i = 0; i < enemyInstList.Num(); ++i)
    {
        int32 SpawnCount = numOfEnemiesPerType.IsValidIndex(i) ? numOfEnemiesPerType[i] : 0;
        for (int32 j = 0; j < SpawnCount; ++j)
        {
            FVector SpawnLocation = FVector(900, 240, 40) + FVector(i * 300, j * 120, 0); // Example: staggered spawn
            FRotator SpawnRotation = FRotator::ZeroRotator;

            SpawnEnemyByIndex(i, SpawnLocation, SpawnRotation);
        }
    }
}

AEnemyCharacterBase* AEnemyGrouperSystem::SpawnEnemyByIndex(int32 TypeIndex, const FVector& Location, const FRotator& Rotation)
{
    if (!enemyInstList.IsValidIndex(TypeIndex) || !numOfEnemiesPerType.IsValidIndex(TypeIndex) || TypeIndex > _amtOfEnemies)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid index for spawning %d"), TypeIndex);
        return nullptr;
    }

    TSubclassOf<AEnemyCharacterBase> EnemyClass = enemyInstList[TypeIndex];
    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy class is null at index %d"), TypeIndex);
        return nullptr;
    }

    UWorld* World = GetWorld();
    if (!World) return nullptr;

    int32 tempIdx = 0;
    for(int32 i = 0; i <= TypeIndex; ++i)
    {
        if (i == TypeIndex)
            tempIdx += _enemyCounter[i];
        else
            tempIdx += numOfEnemiesPerType[i];
    }

    int32 CurrentCount = _enemyCounter.IsValidIndex(TypeIndex) ? _enemyCounter[TypeIndex] : 0;
    FString EnemyName = EnemyClass->GetName() + FString::FromInt(CurrentCount);

    if (CurrentCount > 0 && EnemyName.Compare(enemyNames[tempIdx - 1]) != 0) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Internal spawn name incorrect."));
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName(*EnemyName);

    AEnemyCharacterBase* SpawnedEnemy = World->SpawnActor<AEnemyCharacterBase>(EnemyClass, Location, Rotation, SpawnParams);
    if (SpawnedEnemy)
    {
        _enemyClassMap.Add(EnemyName, SpawnedEnemy);
        UE_LOG(LogTemp, Display, TEXT("Spawned Enemy: %s"), *EnemyName);
        _enemyCounter[TypeIndex]++;
    }

    return SpawnedEnemy;
}

