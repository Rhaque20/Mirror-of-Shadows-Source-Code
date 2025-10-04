// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "EnemySkill.generated.h"

/**
 * 
 */
UCLASS()
class MIRROROFSHADOWS_API UEnemySkill : public USkill
{
	GENERATED_BODY()
	protected:
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Cooldown")
		float MaxCooldown = 0.f;
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Cooldown")
		float StartCooldown = 0.f;
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Cooldown")
		float InternalCooldown = 0.f;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Range Requirement")
		float MinRangeReq = 0;
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Range Requirement")
		float MaxRangeReq = 600;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category ="Range Requirement")
		float AngleReq = 45.0f;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Range Requirement")
		float HeightReq = 100.0f;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		bool bIsReaction = false;
	public:
		float GetMaxCooldown() { return MaxCooldown; }
		float GetStartCooldown() { return StartCooldown; }

		float GetMinRangeReq() { return MinRangeReq; }
		float GetMaxRangeReq() { return MaxRangeReq; }

		float GetAngleReq() { return AngleReq; }
		float GetHeightReq() { return HeightReq; }

		bool IsSkillReaction() { return bIsReaction; }
};
