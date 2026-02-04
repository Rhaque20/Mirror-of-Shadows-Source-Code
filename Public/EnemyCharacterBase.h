// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"

#include "EnemyCharacterBase.generated.h"

class UEnemySkill;
DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FGetTicketDelegate, AEnemyCharacterBase*, Requester, FGameplayTag,
                                          TicketToRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReturnTicketDelegate, AEnemyCharacterBase*, Returner, FGameplayTag, TicketToReturn);
DECLARE_DELEGATE_TwoParams(FReturnTicketFunction, AEnemyCharacterBase*, FGameplayTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDetectedPlayer,AActor*, DetectedActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAggregatedTick,float, DeltaTime);

class UStaggerComponent;

UCLASS()
class MIRROROFSHADOWS_API AEnemyCharacterBase : public ARPGCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();
	UFUNCTION(BlueprintCallable)
	void Recover();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;
	
	UFUNCTION(BlueprintCallable)
	UEnemySkill* GetDecidedSkill() const;

	UFUNCTION(BlueprintCallable)
	void DetectionFill();

	UFUNCTION(BlueprintCallable)
	void GetConditionParameters(AActor *TargetActor, float& Distance, float& Angle, float& Height) const;

	virtual void AutoTarget();

	UFUNCTION(BlueprintCallable)
	void ReceivePlayerTarget(AActor* PlayerRef);

	UFUNCTION(BlueprintCallable)
	void CleanUp();

	UFUNCTION()
	void ReceiveTicket(FGameplayTag TicketToReceive);

	UFUNCTION(BlueprintCallable)
	void ReturnTicket();

	UFUNCTION(BlueprintCallable)
	bool UseSkill();

	UFUNCTION()
	void PreparingForAttack();
	
	void StartDecision();

	UFUNCTION()
	void OnAttackDelayStatusChange(FGameplayTag DelayTag,struct FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    int32 SpawnPortalIdx;

	// FGetTicketDelegate cannot be blueprint callable
	UPROPERTY(BlueprintReadOnly)
	FGetTicketDelegate OnRequestTicket;

	UPROPERTY(BlueprintReadOnly)
	FGetTicketDelegate OnStealTicket;

	UPROPERTY(BlueprintReadOnly,BlueprintCallable)
	FReturnTicketDelegate OnReturnTicket;

	UPROPERTY(BlueprintReadOnly,BlueprintCallable)
	FOnDetectedPlayer OnDetectedPlayer;
	
	UPROPERTY(BlueprintReadOnly,BlueprintAssignable)
	FOnAggregatedTick OnAggregatedTick;



protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool IsAttacking = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool DetectedTarget = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DetectionPercent = 0.0f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DetectionFillRate = 0.1f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Normal Attack")
	TArray<class UEnemySkill*> Moveset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxStatusDisplayDistance = 1200;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	AActor* AttackTarget;

	UPROPERTY()
	class USkillCooldownManagerComponent* SkillCooldownManager;
	
	UPROPERTY()
	class UAsyncTaskEffectStackChanged* AttackDelayListener;
	
	UPROPERTY(BlueprintReadWrite)
	class UAbilityAsync* HitEventWaiter;
	
	UPROPERTY()
	FTimerHandle DecisionTimerHandle;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float DecisionTickInterval = 0.25f;

	FGameplayTag CurrentTicket = FGameplayTag::EmptyTag;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	TArray<FVector> FlyingPositions;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	class UEntityRewardComponent* RewardComponent;
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	bool bCanUseRanged = false;
	
	UPROPERTY()
	bool bCanDecide = true;
	
	UPROPERTY()
	UEnemySkill* SkillToUse;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> UnawareEffectClass;
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	bool bStartUnaware = true;
	
	UPROPERTY()
	FActiveGameplayEffectHandle UnawareEffect;
	
	UPROPERTY()
	TArray<class UActorComponent*> AggregateComponents;


public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	UEnemySkill* ReturnAvailableSkill();

	UFUNCTION(BlueprintCallable)
	void SetSkillCooldown(UEnemySkill* SkillToCD);

	void GiveFlyingPositions(TArray<FVector> FlyingPositions);
	
	UFUNCTION()
	void GroupEnemyTick(float DeltaTime);

};

