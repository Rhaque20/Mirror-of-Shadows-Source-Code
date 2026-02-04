// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibraries/CombatHelperLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/BaseAttributeSet.h"
#include "GAS/UDGameplayTags.h"

FGameplayEffectSpecHandle UCombatHelperLibrary::GenerateDamageSpec(TSubclassOf<UGameplayEffect> DamageEffectClass,
                                                                   UAbilitySystemComponent* OwningASC, float SkillModifier, int HitResult)
{
	FGameplayEffectContextHandle Context = OwningASC->MakeEffectContext();
	FGameplayEffectSpecHandle DamageSpec = OwningASC->MakeOutgoingSpec(DamageEffectClass,1,Context);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpec,TAG_Skill,SkillModifier);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpec,TAG_Event_Combat_Hit,HitResult);
	return DamageSpec;
}

FGameplayEffectSpecHandle UCombatHelperLibrary::GeneratePoiseDamageSpec(
	TSubclassOf<UGameplayEffect> PoiseDMGEffectClass, UAbilitySystemComponent* OwningASC, float PoiseDMG,
	UAbilitySystemComponent* TargetASC)
{
	FGameplayEffectContextHandle Context = OwningASC->MakeEffectContext();
	FGameplayEffectSpecHandle PoiseDamageSpec = OwningASC->MakeOutgoingSpec(PoiseDMGEffectClass,1,Context);
	
	bool bFoundAttr = false;
	float PoiseMod = UAbilitySystemBlueprintLibrary::GetFloatAttributeFromAbilitySystemComponent(TargetASC,UBaseAttributeSet::GetPoiseModAttribute(),bFoundAttr);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PoiseDamageSpec,TAG_Damage_Stagger_Poise,PoiseDMG * PoiseMod);
	
	return PoiseDamageSpec;
}
