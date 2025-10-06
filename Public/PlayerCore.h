// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatComponent.h"
#include "Enumerator/AttackCategoryEnum.h"
#include "GameplayTagContainer.h"

#include "PlayerCore.generated.h"

/**
 * 
 */

class UPlayerSkill;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable )
class MIRROROFSHADOWS_API UPlayerCore : public UCombatComponent
{
	GENERATED_BODY()
	public:
		UPlayerCore();
		virtual void NormalAttack();
		UFUNCTION(BlueprintCallable)
		virtual void Hit() override;
		virtual void SetUpAttackAnim() override;

		UFUNCTION(BlueprintCallable)
		void SetSkillRef(int index);

		UFUNCTION(BlueprintCallable)
		void SetCanBuffer(bool val) { CanBuffer = val; }
		UFUNCTION(BlueprintCallable)
		void SetHasBuffer(bool val) {
			if (val)
			{
				UE_LOG(LogTemp, Display, TEXT("Setting the buffer"));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Removing the buffer"));
			}

			HasBuffer = val;
		}

		UFUNCTION(BlueprintCallable)
		bool ReturnCanBuffer() const { return CanBuffer; }
		UFUNCTION(BlueprintCallable)
		bool ReturnHasBuffer() const { return HasBuffer; }

		UFUNCTION(BlueprintCallable)
		void GetSkillEnergyAtIndex(int Index,float& EnergyRatio, float& CurrentEnergy, float& MaxEnergy, bool OnGround = false) const;

		int GetChainLevelRequirement(bool OnGround) const
		{
			return OnGround ? GroundChainLevelReq : AirChainLevelReq;
		}

		TArray<UPlayerSkill*> GetSkillList(bool OnGround) const
		{
			if (OnGround)
				return GroundSkills;
			else
				return AirSkills;
		}

		UPlayerSkill* GetSwitchSkill(bool OnGround) const
		{
			if (OnGround)
				return GroundSwitchSkill;
			else
				return AirSwitchSkill;
		}

		void FillAllSkillEnergy(float EnergyAmount);
		void FillAllSkillEnergy(float EnergyAmount, FGameplayTag SkillElement, float MatchModifier, float MismatchModifier);

		UFUNCTION(BlueprintCallable)
		bool CanUseSkill(int index, bool bOnGround) const;

		UPlayerSkill* GetHeavyAttack() const { return HeavyAttack; }

	protected:
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Normal Attack")
		TArray<UPlayerSkill*> NormalAttacks;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Normal Attack")
		TArray<UPlayerSkill*> AerialAttacks;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Normal Attack")
		UPlayerSkill* HeavyAttack;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Data")
		TArray<UPlayerSkill*> GroundSkills;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Data")
		TArray<UPlayerSkill*> AirSkills;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Data", meta=(ToolTip="Skill to use during skill chain on ground"))
		UPlayerSkill* GroundSwitchSkill;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Data", meta = (ToolTip = "Skill to use during skill chain in air"))
		UPlayerSkill* AirSwitchSkill;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Data", meta = (ToolTip = "Level required to activate skill chain on ground"))
		int GroundChainLevelReq = 1;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Data", meta = (ToolTip = "Level required to activate skill chain in the air"))
		int AirChainLevelReq = 1;

		class APlayerCharacters* OwningPlayer;

		UPROPERTY(BlueprintReadWrite)
		int CurrentChain = 0;
		UPROPERTY(BlueprintReadWrite)
		bool CanBuffer = false;
		UPROPERTY(BlueprintReadWrite)
		bool HasBuffer = false;
		UPROPERTY(BlueprintReadOnly)
		bool EndOfChain = false;
	
};
