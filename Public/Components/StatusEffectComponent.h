// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Enumerator/StatusEffectEnum.h"

#include "StatusEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRROROFSHADOWS_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	const int BURN = 1, SILENCE = 2, SLOW = 3, POISON = 4, PARALYZE = 5, BLIND = 6;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> BurnStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> SilenceStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> SlowStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> PoisonStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> ParalyzeStatus;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> BlindStatus;

	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly, meta = (ToolTip="0 = Bleed, 1 = Burn, 2 = Silence, 3 = Slow, 4 = Poison, 5 = Paralyze, 6 = Blind"))
	TArray<float> StatusDamageAccumulation = TArray<float>{ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

	UPROPERTY()
	int NumberofActiveEffects;

	UPROPERTY()
	int BurnStacks = 0, PoisonStacks = 0;

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
	

public:	
	// Called every frame
	/*virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;*/

	UFUNCTION()
	void ApplyStatusDamage(FGameplayTag StatusTag,float StatusDamage);

	UFUNCTION()
	void StatusBuildUpDecayTick();

	UFUNCTION()
	void AddDecayTimer(EStatusEffect StatusEnum);

	UFUNCTION()
	void BroadcastStatusBuildUp(float BuildUpAmount, FGameplayTag StatusTag);

protected:


		
};
