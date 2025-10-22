// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enumerator/SkillHeight.h"
#include "Structs/ParryHitStruct.h"
#include "Structs/SkillHitDataStruct.h"
#include "EnumLib.h"
#include "GameplayTagContainer.h"
#include "../../../../Plugins/Wwise/Source/AkAudio/Classes/AkAudioEvent.h"

#include "Skill.generated.h"

/**
 * 
 */
class UAnimationAsset;
class UCurveAsset;
UCLASS(Blueprintable)
class MIRROROFSHADOWS_API USkill : public UDataAsset
{
	GENERATED_BODY()
	protected:
		UPROPERTY(EditDefaultsOnly)
		FString SkillName;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		ESkillHeight SkillHeightRequirement;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class UCharacterGameplayAbility> AbilityClass;

		UPROPERTY(EditDefaultsOnly)
		TArray<UAnimMontage*> AnimationMontages;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stagger System")
		float ResistInterruptMod = 1.f;
		
		UPROPERTY(EditDefaultsOnly)
		bool IsProjectile = false;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Force Settings", meta = (ToolTip = "Does the direction of the force also go on the Z-Axis"))
		bool IsFlying = false;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Force Settings", meta = (ToolTip = "Will activating this skill suspend the target in the air?"))
		bool HasAirSuspension = false;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Force Settings", meta = (ToolTip= "Used for propelling the attacker towards or back"))
		UCurveFloat* LaunchCurve;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Force Settings", meta = (ToolTip = "How strong the self propulsion force is"))
		float LaunchForce = 100.f;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Hit Information")
		TArray<FSkillHitData> HitInformation;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Icon")
		class UPaperSprite* SkillIcon;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Skill")
		FGameplayTag SkillElement;

		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "Effects")
		TArray<TSubclassOf<class UGameplayEffect>> BaseEffects;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Status Effect Influence")
		float ResistanceToSilence = 0.0f;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "State Requirement")
		FGameplayTag StateRequirement;

	public:
		UFUNCTION(BlueprintCallable)
		UAnimMontage* GetAnimation(int index) const;

		UFUNCTION(BlueprintCallable)
		FSkillHitData ReturnSkillHitData(int index) const
		{
			if (HitInformation.Num() == 0)
			{
				return FSkillHitData();
			}

			return HitInformation[index % HitInformation.Num()];
		}


		TSubclassOf<class UCharacterGameplayAbility> ReturnAbilityClass() const
		{
			return AbilityClass;
		}

		UCurveFloat* GetLaunchCurve() const { return LaunchCurve; }
		float GetLaunchForce() const { return LaunchForce; }
		FString GetSkillName() const { return SkillName; }
		FGameplayTag GetSkillElement() const { return SkillElement; }
		FGameplayTag GetStateRequirement() const { return StateRequirement; }
};
