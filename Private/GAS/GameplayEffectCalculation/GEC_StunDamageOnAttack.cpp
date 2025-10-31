// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayEffectCalculation/GEC_StunDamageOnAttack.h"
#include "GAS/BaseAttributeSet.h"
#include "GAS/EnemyAttributeSet.h"
#include "GAS/PlayerAttributeSet.h"

struct FStunDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(ATK);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ATKBonusPercent);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DEF);
    DECLARE_ATTRIBUTE_CAPTUREDEF(SkillModifier);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DMGRes);
    DECLARE_ATTRIBUTE_CAPTUREDEF(StunDMG);
    DECLARE_ATTRIBUTE_CAPTUREDEF(StunDMGUp);
    DECLARE_ATTRIBUTE_CAPTUREDEF(HitResultModifier);

    FStunDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ATK, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ATKBonusPercent, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DEF, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, SkillModifier, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DMGRes, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UEnemyAttributeSet, StunDMG, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, StunDMGUp, Source, false);
        // We want to snapshot the value at the time this effect got calculated
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, HitResultModifier, Source, true);
    }
};

static const FStunDamageStatics StunDamageStatics()
{
    static FStunDamageStatics DMGStatics;
    return DMGStatics;
}

UGEC_StunDamageOnAttack::UGEC_StunDamageOnAttack()
{
    RelevantAttributesToCapture.Add(StunDamageStatics().DEFDef);
    RelevantAttributesToCapture.Add(StunDamageStatics().ATKDef);
    RelevantAttributesToCapture.Add(StunDamageStatics().StunDMGUpDef);
    RelevantAttributesToCapture.Add(StunDamageStatics().StunDMGDef);
    RelevantAttributesToCapture.Add(StunDamageStatics().SkillModifierDef);
    RelevantAttributesToCapture.Add(StunDamageStatics().DMGResDef);
    RelevantAttributesToCapture.Add(StunDamageStatics().ATKBonusPercentDef);
    RelevantAttributesToCapture.Add(StunDamageStatics().HitResultModifierDef);
}

void UGEC_StunDamageOnAttack::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
    AActor* OwningActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    // Skill Modifier will be the stun modifier of the attack.
    float SkillModifier = 0.0f;
    float HitResultModifier = 0.0f;

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StunDamageStatics().SkillModifierDef, EvaluationParameters, SkillModifier);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StunDamageStatics().HitResultModifierDef, EvaluationParameters, HitResultModifier);

    FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();

    float FinalAttack = 0.0f;
    float FinalDefense = 0.0f;
    float ATKBonusPercent = 0.0f;
    float FinalDMGRes = 0.0f;
    float StunDMGUp = 0.0f;

    int DidHit = (int)HitResultModifier;// -1 = Missed, 0 = Hit, 1 = Critical Hit

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StunDamageStatics().ATKDef, EvaluationParameters, FinalAttack);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StunDamageStatics().ATKBonusPercentDef, EvaluationParameters, ATKBonusPercent);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StunDamageStatics().DEFDef, EvaluationParameters, FinalDefense);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StunDamageStatics().DMGResDef, EvaluationParameters, FinalDMGRes);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StunDamageStatics().StunDMGUpDef, EvaluationParameters, StunDMGUp);


    FinalDMGRes = FMath::Clamp(FinalDMGRes, -1.0f, 0.75f);

    UE_LOG(LogTemp, Display, TEXT("Attacker's Attack: %f and Defender's Defense: %f"), FinalAttack, FinalDefense);

    FinalAttack *= (1 + ATKBonusPercent);

    UE_LOG(LogTemp, Display, TEXT("Attacker's Attack with ATK Bonus: %f"), FinalAttack);

    float FinalDamage = 0.0f;
    FinalDamage = (FinalAttack * SkillModifier) / (FinalDefense / 300 + 1);
    FinalDamage *= (1 - FinalDMGRes) * (1+StunDMGUp);

    if (DidHit < 0)// If the hit was registered as a miss, reduce it to 70%
    {
        FinalDamage *= 0.7f;
    }

    FinalDamage = FMath::Floor(FinalDamage);

    UE_LOG(LogTemp, Display, TEXT("Dealt %f Stun Damage to %s"), FinalDamage, *(TargetActor->GetName()));

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(StunDamageStatics().StunDMGProperty, EGameplayModOp::AddBase, FinalDamage));
}
