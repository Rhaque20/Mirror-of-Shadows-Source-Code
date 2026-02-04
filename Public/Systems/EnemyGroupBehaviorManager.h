// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "EngineSystems/EnemyManagerTick.h"

#include "EnemyGroupBehaviorManager.generated.h"

class AEnemyCharacterBase;

struct EnemyScore
{
	AEnemyCharacterBase* EnemyRef;
	float ScoreForDistance = 0.0f;
	float ScoreByMissedTurns = 0.0f;

	bool operator<(const EnemyScore& y) const
	{
		return (ScoreForDistance + ScoreByMissedTurns) > (y.ScoreForDistance + y.ScoreByMissedTurns);
	}
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleBegin,AActor*,PlayerRef);
UCLASS()
class MIRROROFSHADOWS_API AEnemyGroupBehaviorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyGroupBehaviorManager();
	
	void SetTicketAmounts(int MaxMeleeTickets, int MaxRangeTickets);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TMap<AEnemyCharacterBase*, FGameplayTag> TicketTracker;
	
	UPROPERTY(BlueprintAssignable)
	FOnBattleBegin OnBattleBegin;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy Ticket System")
	int MeleeTickets = 2;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy Ticket System")
	int RangeTickets = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Ticket System")
	TSubclassOf<class UGameplayEffect> MeleeTicketEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Ticket System")
	TSubclassOf<class UGameplayEffect> RangeTicketEffect;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Enemy List")
	// Map of enemies in the group with their respective scores.
	TArray<AEnemyCharacterBase*> EnemiesInGroup;

	TMap<AEnemyCharacterBase*,EnemyScore*> EnemyScoreboard;

	// How often will this tick?
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float LoopFrequency = 0.5f;

	// Distance to be considered close for melee
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Ticket System Parameters")
	float ClosenessDistanceForMelee = 250.0f;

	// Distance to be considered close for melee
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ticket System Parameters")
	float ClosenessDistanceForRange = 250.0f;

	// How many additional points are given for those who were not granted tickets?
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ticket System Parameters")
	float ScoreBoostForMissedTurns = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ticket System Parameters")
	float ScoreBoostForMissedEliteTurns = 100.0f;

	bool bDetectedPlayer = false;

	// Tags to look out for to reset their score.
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category = "Ticket System Parameters")
	FGameplayTagContainer ResetTags;

	FTimerHandle GroupLoopHandle;

public:	

	UFUNCTION()
	bool GiveTicket(AEnemyCharacterBase* Requester, FGameplayTag TicketType);

	UFUNCTION()
	void ReturnTicket(AEnemyCharacterBase* Requester, FGameplayTag TicketType);

	UFUNCTION()
	void StealTicket(AEnemyCharacterBase* Requester, FGameplayTag TicketType);

	void ReceiveEnemy(AEnemyCharacterBase* Enemy);
	UFUNCTION()
	void RemoveEnemy(class ARPGCharacterBase* Enemy);

	UFUNCTION()
	void DetectedPlayer(AActor* DetectedPlayer);
	
	UFUNCTION()
	void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void EnemyGroupLoop();
};
