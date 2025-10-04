// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GEC_AttackScale.h"
#include "GAS/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPGCharacterBase.h"
#include "GAS/UDGameplayTags.h"

struct FDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(DEF);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ATK);
    DECLARE_ATTRIBUTE_CAPTUREDEF(HP);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ATKBonusPercent);
    DECLARE_ATTRIBUTE_CAPTUREDEF(SkillModifier);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DMGRes);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CritRate);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CritDMG);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ElementDMGBoost);
    DECLARE_ATTRIBUTE_CAPTUREDEF(HitResultModifier);

    FDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,HP, Source,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,ATK, Source,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,CritRate, Source,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,CritDMG, Source,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,SkillModifier, Source,true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,ATKBonusPercent,Source,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,DMGRes,Target,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,BlockPower,Target,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,Damage, Target,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet,DEF, Target,false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ElementDMGBoost, Source, false);
        // We want to snapshot the value at the time this effect got calculated
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, HitResultModifier, Source, true);
    }
};

static const FDamageStatics DamageStatics()
{
    static FDamageStatics DMGStatics;
    return DMGStatics;
}

UGEC_AttackScale::UGEC_AttackScale() 
{
	RelevantAttributesToCapture.Add(DamageStatics().DEFDef);
    RelevantAttributesToCapture.Add(DamageStatics().ATKDef);
    RelevantAttributesToCapture.Add(DamageStatics().HPDef);
    RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().SkillModifierDef);
    RelevantAttributesToCapture.Add(DamageStatics().DMGResDef);
    RelevantAttributesToCapture.Add(DamageStatics().BlockPowerDef);
    RelevantAttributesToCapture.Add(DamageStatics().ATKBonusPercentDef);
    RelevantAttributesToCapture.Add(DamageStatics().CritRateDef);
    RelevantAttributesToCapture.Add(DamageStatics().CritDMGDef);
    RelevantAttributesToCapture.Add(DamageStatics().ElementDMGBoostDef);
    RelevantAttributesToCapture.Add(DamageStatics().HitResultModifierDef);
}

bool UGEC_AttackScale::IsBlocked(AActor* Target, AActor* Attacker) const
{
    FVector AttackerVector = Attacker->GetActorForwardVector();
	FVector TargetLocation = Target->GetActorForwardVector();

	double dotProduct = UKismetMathLibrary::Dot_VectorVector(TargetLocation,AttackerVector);
	return dotProduct <= -0.7f;
}

void UGEC_AttackScale::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const 
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
    float ElementBoost = 0.0f;
    float HitResultModifier = 0.0f;

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SkillModifierDef, EvaluationParameters, SkillModifier);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ElementDMGBoostDef, EvaluationParameters, ElementBoost);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HitResultModifierDef, EvaluationParameters, HitResultModifier);

    if (ElementBoost > 0.0f)
    {
        UE_LOG(LogTemp, Display, TEXT("Element damage boost for specific element has been applied and it's modifier is %f"),ElementBoost);
    }

    FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();

    float FinalAttack = 0.0f;
    float FinalDefense = 0.0f;
    float FinalDamage = 0.0f;
    float ATKBonusPercent = 0.0f;
    float FinalDMGRes = 0.0f;
    float BlockPower = 0.0f;
    float CriticalChance = 0.05f;
    float CriticalDMG = 0.0f;
    int DidHit = (int)HitResultModifier;// -1 = Missed, 0 = Hit, 1 = Critical Hit

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ATKDef, EvaluationParameters, FinalAttack);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ATKBonusPercentDef, EvaluationParameters, ATKBonusPercent);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DEFDef, EvaluationParameters, FinalDefense);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DMGResDef, EvaluationParameters, FinalDMGRes);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritRateDef, EvaluationParameters, CriticalChance);

    if (IsBlocked(TargetActor,OwningActor) && TargetASC->HasMatchingGameplayTag(TAG_Effect_Defensive_Block))
    {
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockPowerDef, EvaluationParameters, BlockPower);
    }

    if (DidHit >= 0)
    {
        if (FMath::RandRange(1.0f, 100.0f) <= CriticalChance)
        {
            UE_LOG(LogTemp, Display, TEXT("CRITICAL HIT!"));
            ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDMGDef, EvaluationParameters, CriticalDMG);
            CriticalDMG = FMath::Max(CriticalDMG, 0.5f);
            DidHit = 1;
        }
    }

    FinalDMGRes = FMath::Clamp(FinalDMGRes,-1.0f,0.75f);

    UE_LOG(LogTemp, Display, TEXT("Attacker's Attack: %f and Defender's Defense: %f"),FinalAttack,FinalDefense);

    FinalAttack *= (1 + ATKBonusPercent);

    UE_LOG(LogTemp, Display, TEXT("Attacker's Attack with ATK Bonus: %f"),FinalAttack);

    FinalDamage = (FinalAttack * SkillModifier)/(FinalDefense/300 + 1);
    FinalDamage *= (1 - FinalDMGRes) * (1 - BlockPower) * (1 + CriticalDMG) * (1+ElementBoost);

    if (DidHit < 0)// If the hit was registered as a miss, reduce it to 70%
    {
        FinalDamage *= 0.7f;
    }

    FinalDamage = FMath::Floor(FinalDamage);

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty,EGameplayModOp::AddBase, FinalDamage));

    Cast<ARPGCharacterBase>(TargetActor)->DisplayDamageNumber(*SourceTags,FinalDamage,DidHit);

}
