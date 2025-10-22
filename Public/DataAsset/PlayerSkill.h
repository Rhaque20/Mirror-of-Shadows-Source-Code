// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "PlayerSkill.generated.h"

/**
 * 
 */
UCLASS()
class MIRROROFSHADOWS_API UPlayerSkill : public USkill
{
	GENERATED_BODY()
	protected:
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="SP System")
		bool bHasNRGCost = true;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SP System", meta = (EditCondition = "bHasNRGCost", EditConditionHides))
		bool bInfiniteEnergy = false;

		UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bHasNRGCost", EditConditionHides, ClampMin = 1.0f), Category = "SP System")
		float EnergyCost = 1.f;
		UPROPERTY(BlueprintReadWrite, EditDefaultsOnly,meta = (EditCondition = "bHasNRGCost", EditConditionHides), Category = "SP System")
		float CurrentEnergy = 0.f;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,meta = (EditCondition = "!bHasNRGCost", EditConditionHides), Category = "SP System")
		float SPGain = 0.f;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta =(ToolTip = "Key is the level required to proc the effect and the value is the effect it will trigger"))
		TMap<int, class USkillChainSlot*> EffectSlots;

		bool bCanGainSkillEnergy = true;

	public:
		UFUNCTION(BlueprintCallable)
		bool IsReadyToUse() const { return bInfiniteEnergy || SkillEnergyRatio() >= 1.0f; }
		UFUNCTION(BlueprintCallable)
		float SkillEnergyRatio() const { return CurrentEnergy / EnergyCost; }
		UFUNCTION(BlueprintCallable)
		float SkillEnergyRatio_UI() const { return 1 - (SkillEnergyRatio()); }

		UFUNCTION(BlueprintCallable)
		void GetSkillEnergyInfo(float& CurrentNRG, float& NRGCost) const
		{
			CurrentNRG = CurrentEnergy;
			NRGCost = EnergyCost;
		}

		UFUNCTION(BlueprintCallable)
		float GetSkillEnergyCost() const
		{
			return EnergyCost;
		}
		UFUNCTION(BlueprintCallable)
		void FillSkillEnergy(float EnergyGain);
		UFUNCTION(BlueprintCallable)
		void ExpendSkillEnergy(float EnergyUseRatio = 1.0f);

		UFUNCTION(BlueprintCallable)
		void SetCanGainSkillEnergy(bool val)
		{
			bCanGainSkillEnergy = val;
		}
};
