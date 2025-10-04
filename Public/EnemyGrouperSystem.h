// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include "GameFramework/Actor.h"
#include "EnemyCharacterBase.h"
#include "EnemyGrouperSystem.generated.h"

//using EnemyMap = TMap<FString, AEnemyCharacterBase*>;

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
    AEnemyCharacterBase* SpawnEnemyByIndex(int32 TypeIndex, const FVector& Location, const FRotator& Rotation);

    UPROPERTY(EditAnywhere, Category = "Enemy Spawning")
    bool spawnAllEnemiesAtOnce = false;

    UPROPERTY(EditInstanceOnly, Category = "Enemy Types")
    TArray<TSubclassOf<AEnemyCharacterBase>> enemyInstList;

    UPROPERTY(EditInstanceOnly, Category = "Enemy Types")
    TArray<int32> numOfEnemiesPerType;

    UPROPERTY(VisibleInstanceOnly, Category = "Enemy Types")
    TArray<FString> enemyNames;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    TMap<FString, AEnemyCharacterBase*> _enemyClassMap;
    TArray<int32> _enemyCounter;
    int32 _amtOfEnemies;
};
