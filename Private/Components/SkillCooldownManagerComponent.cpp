// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillCooldownManagerComponent.h"
#include "DataAsset/EnemySkill.h"

// Sets default values for this component's properties
USkillCooldownManagerComponent::USkillCooldownManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillCooldownManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USkillCooldownManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillCooldownManagerComponent::SetUpSkillCooldowns(TArray<UEnemySkill*> Skills)
{
	SkillCooldowns = TMap<UEnemySkill*, float>();

	for (UEnemySkill* skillData : Skills)
	{
		float startCD = skillData->GetStartCooldown();
		SkillCooldowns.Add(skillData, startCD);
	}

	GetWorld()->GetTimerManager().SetTimer
	(CooldownTimer, 
		this, 
		&USkillCooldownManagerComponent::SkillCooldownTick, 
		0.5f, 
		true);

	SkillsOwned = Skills;
}

void USkillCooldownManagerComponent::SetSkillCooldown(UEnemySkill* SkillRef)
{
	if (SkillCooldowns.Contains(SkillRef))
	{
		SkillCooldowns[SkillRef] = SkillRef->GetMaxCooldown();
	}
}

bool USkillCooldownManagerComponent::IsSkillOnCooldown(UEnemySkill* SkillRef) const
{
	if (!SkillCooldowns.Contains(SkillRef))
		return false;

	return SkillCooldowns[SkillRef] > 0.0f;
}

void USkillCooldownManagerComponent::SkillCooldownTick()
{
	for (UEnemySkill* skill : SkillsOwned)
	{
		if(SkillCooldowns[skill] > 0.0f)
			SkillCooldowns[skill] -= 0.5f;
	}
}

