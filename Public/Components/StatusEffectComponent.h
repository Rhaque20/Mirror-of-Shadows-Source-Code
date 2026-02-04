// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Enumerator/StatusEffectEnum.h"
#include "Interfaces/AggregateTickInterface.h"

#include "StatusEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRROROFSHADOWS_API UStatusEffectComponent : public UActorComponent, public IAggregateTickInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	const int DAZE = 0, BURN = 1, SILENCE = 2, SLOW = 3, FROZEN = 4, SHOCK = 5, BLIND = 6;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> DazeStatus;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> BurnStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> SilenceStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> SlowStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> FreezeStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> ShockStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> CurseStatus;

	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly, meta = (ToolTip="0 = Daze, 1 = Burn, 2 = Silence, 3 = Slow, 4 = Frozen, 5 = Shock, 6 = Curse"))
	TArray<float> StatusDamageAccumulation = TArray<float>{ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

	UPROPERTY()
	int NumberofActiveEffects;

	UPROPERTY()
	int BurnStacks = 0, ShockStacks = 0;

	UPROPERTY(VisibleDefaultsOnly)
	class UAbilitySystemComponent* OwnerAbilitySystem;

	UPROPERTY()
	FTimerHandle StatusBuildUpDecayTimer;

	UPROPERTY()
	TMap<EStatusEffect, float> StatusDecayTimeRemainingMap;

	float DecayInterval = 0.25f;
	float TimeUntilDecay = 10.0f;


	UPROPERTY()
	float DecayPerSecond = 5.0f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bUseAggregate = false;
	

public:	
	// Called every frame
	/*virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;*/

	UFUNCTION()
	void ApplyStatusDamage(class UAbilitySystemComponent* Applier,FGameplayTag StatusTag,float StatusDamage);

	UFUNCTION()
	void StatusBuildUpDecayTick();

	UFUNCTION()
	void AddDecayTimer(EStatusEffect StatusEnum);

	UFUNCTION()
	void BroadcastStatusBuildUp(float BuildUpAmount, FGameplayTag StatusTag);
	
	UFUNCTION()
	void AggregateTick(float DeltaTime) override;


		
};
