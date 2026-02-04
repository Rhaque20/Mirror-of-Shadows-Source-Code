// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeNodeData.generated.h"

UENUM(BlueprintType)
enum class ESkillNodeCategory : uint8
{
	StatBoost UMETA(DisplayName = "Stat Boost"),
	AbilityGranted UMETA(DisplayName = "Ability Granted"),
	SkillGranted UMETA(DisplayName = "Skill Granted"),
	SkillRemixed UMETA(DisplayName = "Skill Remixed"),
	SkillChainSlot UMETA(DisplayName = "SkillChainSlot")
};

UCLASS(Blueprintable)
class MIRROROFSHADOWS_API USkillTreeNodeData : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Node Effect", meta = (EditConditionHides,EditConditions="SkillNodeCategory == ESkillNodeCategory::StatBoost"))
	TSubclassOf<class UGameplayEffect> GrantedEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Node Effect", meta = (EditConditionHides,EditConditions="SkillNodeCategory == ESkillNodeCategory::AbilityGranted"))
	TSubclassOf<class UGameplayAbility> GrantedAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Node Effect", meta = (EditConditionHides,EditConditions="SkillNodeCategory == ESkillNodeCategory::SkillChainSlot"))
	class USkillChainSlot* GrantedSkillChainSlot;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Node Effect", meta = (EditConditionHides,EditConditions="SkillNodeCategory == ESkillNodeCategory::SkillGranted"))
	class UPlayerSkill* GrantedSkill;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Node Effect")
	ESkillNodeCategory SkillNodeCategory = ESkillNodeCategory::StatBoost;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Icon")
	class UPaperSprite* SlotEffectIcon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Name")
	FText SkillNodeName;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Description")
	FText Description;


public:
	TSubclassOf<UGameplayEffect> GetGrantedEffect() const { return GrantedEffect; }
	TSubclassOf<UGameplayAbility> GetGrantedAbility() const { return GrantedAbility; }
	USkillChainSlot* GetGrantedSkillChainSlot() const { return GrantedSkillChainSlot; }
	UPlayerSkill* GetPlayerSkill() const {return GrantedSkill;}
	ESkillNodeCategory GetSkillNodeCategory() const { return SkillNodeCategory; }
		
};
