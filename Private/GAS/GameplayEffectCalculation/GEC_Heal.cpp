// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayEffectCalculation/GEC_Heal.h"

// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPGCharacterBase.h"
#include "GAS/UDGameplayTags.h"

struct FHealStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(TotalHP);
    DECLARE_ATTRIBUTE_CAPTUREDEF(HealBonus);
    DECLARE_ATTRIBUTE_CAPTUREDEF(SkillModifier);
    DECLARE_ATTRIBUTE_CAPTUREDEF(HealthRestored);

    FHealStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,TotalHP, Target,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,HealBonus, Target,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,SkillModifier,Target,true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,HealthRestored,Target,false);
    }
};

static const FHealStatics HealStatics()
{
    static FHealStatics DMGStatics;
    return DMGStatics;
}

UGEC_Heal::UGEC_Heal() 
{
	RelevantAttributesToCapture.Add(HealStatics().TotalHPDef);
    RelevantAttributesToCapture.Add(HealStatics().HealBonusDef);
    RelevantAttributesToCapture.Add(HealStatics().SkillModifierDef);
    RelevantAttributesToCapture.Add(HealStatics().HealthRestoredDef);
}

void UGEC_Heal::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const 
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
    AActor* OwningActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags. GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    // Retrieve the InDamage value passed to Assign Tag Set By Caller Magnitude (On output of Make Outgoing Spec)
    float SkillModifier = 0.0f;
    float TotalHP = 0.0f;
    float HealBonus = 0.0f;

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().SkillModifierDef, EvaluationParameters, SkillModifier);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().TotalHPDef, EvaluationParameters, TotalHP);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().HealBonusDef, EvaluationParameters, HealBonus);

    FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();

    float FinalHeal = (TotalHP * SkillModifier) * (1+HealBonus);

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealStatics().HealthRestoredProperty,EGameplayModOp::AddBase, FinalHeal));

    // Cast<ARPGCharacterBase>(TargetActor)->DisplayDamageNumber(*SourceTags,FinalDamage,DidHit);

}
