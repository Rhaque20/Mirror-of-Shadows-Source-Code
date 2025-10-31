// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterBase.h"
#include "AbilitySystemInterface.h"

#include "EnemyCharacterBase.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FGetTicketDelegate, AEnemyCharacterBase*, Requester, FGameplayTag, TicketToRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReturnTicketDelegate, AEnemyCharacterBase*, Returner, FGameplayTag, TicketToReturn);
DECLARE_DELEGATE_TwoParams(FReturnTicketFunction, AEnemyCharacterBase*, FGameplayTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDetectedPlayer,AActor*, DetectedActor);

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
	void DetectionFill();

	UFUNCTION(BlueprintCallable)
	void GetConditionParameters(AActor *TargetActor, float& Distance, float& Angle, float& Height) const;

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void SelectAttack(float Distance, float Angle, float Height, bool& Selected);

	virtual void AutoTarget();

	UFUNCTION(BlueprintCallable)
	void ReceivePlayerTarget(AActor* PlayerRef);

	UFUNCTION(BlueprintCallable)
	void CleanUp();

	UFUNCTION()
	void ReceiveTicket(FGameplayTag TicketToReceive);

	UFUNCTION(BlueprintCallable)
	void ReturnTicket();

	UFUNCTION()
	void UseSkill(UEnemySkill* Skill);

	UFUNCTION()
	void PreparingForAttack();

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

	class USkillCooldownManagerComponent* SkillCooldownManager;

	class UAsyncTaskEffectStackChanged* AttackDelayListener;

	FTimerHandle DecisionTimerHandle;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float DecisionTickInterval = 0.25f;

	FGameplayTag CurrentTicket = FGameplayTag::EmptyTag;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	TArray<FVector> FlyingPositions;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	UEnemySkill* ReturnAvailableSkill();

	UFUNCTION(BlueprintCallable)
	void SetSkillCooldown(UEnemySkill* SkillToCD);

	void GiveFlyingPositions(TArray<FVector> FlyingPositions);

};

