// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillCooldownManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRROROFSHADOWS_API USkillCooldownManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillCooldownManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<class UEnemySkill*, float> SkillCooldowns;

	FTimerHandle CooldownTimer;

	TArray<UEnemySkill*> SkillsOwned;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetUpSkillCooldowns(TArray<UEnemySkill*> Skills);

	UFUNCTION(BlueprintCallable)
	void SetSkillCooldown(UEnemySkill* SkillRef);

	UFUNCTION(BlueprintCallable)
	bool IsSkillOnCooldown(UEnemySkill* SkillRef) const;

	UFUNCTION()
	void SkillCooldownTick();
		
};
