// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatHelperLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MIRROROFSHADOWS_API UCombatHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
		UFUNCTION(BlueprintCallable, Category = "Damage System")
		static FGameplayEffectSpecHandle GenerateDamageSpec(TSubclassOf<class UGameplayEffect> DamageEffectClass, class UAbilitySystemComponent* OwningASC,float SkillModifier, int HitResult = 0);
		static FGameplayEffectSpecHandle GeneratePoiseDamageSpec(TSubclassOf<class UGameplayEffect> PoiseDMGEffectClass, class UAbilitySystemComponent* OwningASC,float PoiseDMG, UAbilitySystemComponent* TargetASC);
};
