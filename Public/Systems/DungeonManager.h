// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enumerator/ItemEnum.h"
#include "GameFramework/Actor.h"
#include "DungeonManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictory);
UCLASS()
class MIRROROFSHADOWS_API ADungeonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonManager();

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

protected:
	UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
	TArray<class AItemChest*> ItemChestsReferences;
	// This is for save data purposes.
	TArray<bool> bItemChestClaimed;

	UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
	TArray<class AEnemySpawnerSystem*> EnemyGroupReferences;
	// This is for save data purposes.
	TArray<bool> bEnemyGroupsCleared;

	UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
	class ACheckPointSystem* CheckPointSystem;

	UPROPERTY(BlueprintReadOnly,EditInstanceOnly, Category="Loot Parameters")
	TMap<EGrade,float> Tier1LootGrade;

	UPROPERTY(BlueprintReadOnly,EditInstanceOnly, Category="Loot Parameters")
	TMap<EGrade,float> Tier2LootGrade;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FName BossTag = "Boss";

	UPROPERTY(BlueprintReadOnly,EditInstanceOnly, Category="Loot Parameters")
	FGameplayTag SetTag;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<class UUserWidget> GameOverWidget;

public:
	UPROPERTY(BlueprintAssignable)
	FOnVictory OnVictory;
	

};
