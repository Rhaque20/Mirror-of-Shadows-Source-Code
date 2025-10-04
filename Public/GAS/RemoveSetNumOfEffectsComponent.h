// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectComponent.h"
#include "RemoveSetNumOfEffectsComponent.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Remove Set Amount Of Effects")
class MIRROROFSHADOWS_API URemoveSetNumOfEffectsComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

	public:
		URemoveSetNumOfEffectsComponent();
		virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
#if WITH_EDITOR
		/**
		 * Warn about periodic Gameplay Effects, that you probably instead want the OngoingTagRequirements in TagRequirementsGameplayEffectComponent
		 */
		virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

		UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1"))
		int AmountOfEffectsToRemove = 1;

		UPROPERTY(EditDefaultsOnly)
		FGameplayTag EffectTagToRemove;
	
};
