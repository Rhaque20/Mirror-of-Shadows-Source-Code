// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "EnumLib.h"

#include "PlayerPartyController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillChainTimeDelegate, float, timeRatio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUnisonGaugeChange, float, SubGaugeRatio, float, MainGaugeRatio, UPaperSprite*,Icon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillChainAdvanceDelegate, int, chainLevel, EElement, SkillElement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
/**
 * 
 */
class UPlayerData;
class APlayerCharacters;
class UGameplayEffect;

UCLASS()
class MIRROROFSHADOWS_API APlayerPartyController : public APlayerController,public IAbilitySystemInterface
{
	GENERATED_BODY()
	public:
		APlayerPartyController();
		void SetUpMembers(TArray<APlayerCharacters*> PartyList);
		int GetSkillChainLevel() const
		{
			return SkillChainLevel;
		}

		virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
		{
			return AbilitySystem;
		}

		UFUNCTION(BlueprintCallable)
		void IncreaseSkillChainLevel(EElement SkillElement);

		UFUNCTION(BlueprintCallable, meta = (ToolTip="Used for freezing or continuing the chain timer"))
		void SetChainTimerPlaying(bool bTimerIsPlaying);

	protected:
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category =" Lock On System")
		bool IsLockedOn = false;
		UPROPERTY(BlueprintReadWrite, Category =" Lock On System")
		AActor* LockOnTarget;
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		int CurrentCharacterIndex = 0;
		int AliveMembersRemaining = 0;

		UPROPERTY(BlueprintReadOnly)
		float UnisonGaugeAmount = 0.0f;

		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "Unison Gauge")
		float UnisonGaugeMax = 500.0f;

		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "Unison Gauge")
		float UnisonGaugeGainBase = 10.0f;

		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "Unison Gauge", meta =(ToolTip="This is for every level AFTER 1"))
		float UnisonGaugeGainPerLevel = 10.0f;

		UPROPERTY(BlueprintReadOnly)
		int CurrentUnisonSkill = -1;

		UPROPERTY(BlueprintReadOnly)
		float CurrentSkillCost = 0.0f;

		class UPaperSprite* SkillIcon;

		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "Unison Gauge")
		TArray<class UUnisonSkill*> UnisonSkills;

		UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int SkillChainLevel = 0;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float MaxChainTime = 10.0f;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float ChainTimerInterval = 0.02f;

		UPROPERTY()
		float RemainingChainTime = 0.0f;

		UPROPERTY(EditDefaultsOnly)
		float SummonRadius = 100.0f;

		// Used to determine how much skill energy is given back to all members of the party
		UPROPERTY(EditDefaultsOnly, meta = (ToolTip="Used to determine how much skill energy is given back to all members of the party"))
		float EnergyRefundRatio = 0.25f;

		UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Used to determine how much skill energy is given back to all members of the party for mismatched elements"))
		float EnergyRefundRatioMismatch = 0.125f;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		APlayerCharacters* ActiveCharacter;

		bool bExtendChainThree = true;
		bool bIsTimerActive = true;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool bGodMode = false;

		UPROPERTY(BlueprintReadWrite)
		TArray<APlayerCharacters*> SummonedActorReferences;

		TArray<bool> bIsPlayerDead;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UPlayerData*> PartyMemberData;

		UPROPERTY()
		FTimerHandle ChainTimerHandle;

		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "Gameplay Effects")
		TSubclassOf<UGameplayEffect> OfffieldState;

		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "Gameplay Effects")
		TSubclassOf<UGameplayEffect> ForceSwapImmunityEffect;

		UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "Gameplay Effects")
		UAbilitySystemComponent* AbilitySystem;
	protected:
		virtual void BeginPlay();
		UFUNCTION(BlueprintCallable)
		void SwapCharacterLeft();
		UFUNCTION(BlueprintCallable)
		void SwapCharacterRight();

		int GetCharacterLeftIndex();
		int GetCharacterRightIndex();

		UFUNCTION(BlueprintCallable)
		void SummonAttackLeft();
		UFUNCTION(BlueprintCallable)
		void SummonAttackRight();

		UFUNCTION()
		void ChainTimerUpdate();

		UFUNCTION()
		void OnPlayerDeath(class ARPGCharacterBase* PlayerCharacter);

		UFUNCTION(BlueprintCallable)
		void ActivateUnisonSkill();

		void StartChainTimer();

		void EndChainTimer();

		void SummonAttack(int index);

		void SwapCharacter(int SwapToIndex);

		UFUNCTION(BlueprintCallable)
		void ApplyGameplayEffectToAllAllies(FGameplayEffectSpecHandle Handle);

		UFUNCTION(BlueprintCallable)
		void RemoveGameplayEffectForAllAllies(FGameplayTag RemovalTag);

		UFUNCTION()
		void GiveSPToOtherAllies(APlayerCharacters* ActivatingActor, class UPlayerSkill* SkillActivated);

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SwitchResponse(); // Used for switch parries, attack switching, ordinary switches, and counter switch

	public:
		UPROPERTY(BlueprintAssignable, Category = "SkillChainTimeUpdate")
		FSkillChainTimeDelegate SkillChainTimeUpdate;

		UPROPERTY(BlueprintAssignable, Category = "SkillChainTimeUpdate")
		FSkillChainAdvanceDelegate SkillChainAdvance;

		UPROPERTY(BlueprintAssignable,BlueprintCallable)
		FOnGameOver GameOver;

		UPROPERTY(BlueprintAssignable,BlueprintCallable)
		FUnisonGaugeChange OnUnisonGaugeChange;
};
