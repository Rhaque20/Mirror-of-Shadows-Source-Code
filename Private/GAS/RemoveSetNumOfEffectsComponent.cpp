// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RemoveSetNumOfEffectsComponent.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "RemoveSetAmountOfGameplayEffectsComponent"

URemoveSetNumOfEffectsComponent::URemoveSetNumOfEffectsComponent()
{
	#if WITH_EDITORONLY_DATA
		EditorFriendlyName = TEXT("Remove Set Amount Of Gameplay Effects");
	#endif
}

/// <summary>
/// When the effect is applied, will try to remove up to N amount of gameplay effects
/// that match the tag to remove. N being defined as AmountOfEffectsToRemove
/// </summary>
/// <param name="ActiveGEContainer">The effect container it is being put in</param>
/// <param name="GESpec">The owner's effect spec</param>
/// <param name="PredictionKey">Probably multiplayer stuff</param>
void URemoveSetNumOfEffectsComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (!ActiveGEContainer.OwnerIsNetAuthority)
	{
		return;
	}

	FGameplayEffectQuery FindOwnerQuery;
	FindOwnerQuery.EffectDefinition = GetOwner() ? GetOwner()->GetClass() : nullptr;

	// We need to keep track to ensure we never remove ourselves
	TArray<FActiveGameplayEffectHandle> ActiveGEHandles = ActiveGEContainer.GetActiveEffects(FindOwnerQuery);
	FGameplayTagContainer AssetTags;

	int remainingEffectsToRemove = AmountOfEffectsToRemove;
	constexpr int32 RemoveAllStacks = -1;

	while (remainingEffectsToRemove > 0)
	{
		// This keeps track of the longest effect, what was the highest duration
		// And if we removed a gameplay effect (used to avoid infinite loops)
		FActiveGameplayEffectHandle longestEffect;
		float HighestDuration = -1, curDuration = 0.0f;;
		bool hasRemoved = false;

		for (FActiveGameplayEffectHandle curHandle : ActiveGEHandles)
		{
			FActiveGameplayEffect* activeEffect = ActiveGEContainer.GetActiveGameplayEffect(curHandle);

			activeEffect->Spec.GetAllAssetTags(AssetTags);

			// If the active effect has the matching tag for removal
			if (AssetTags.HasTag(EffectTagToRemove))
			{
				curDuration = activeEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds());
				// Check to see if this effect has the longest remaining time compared to previously
				// compared ones
				if (curDuration > HighestDuration)
				{
					longestEffect = curHandle;
					HighestDuration = curDuration;
				}

			}
		}

		// If we had found an effect to remove
		if (HighestDuration >= 0.0f)
		{
			hasRemoved = true;
			ActiveGEContainer.RemoveActiveGameplayEffect(longestEffect, RemoveAllStacks);
			remainingEffectsToRemove--;

			// If we removed N amount of effects end here
			if (remainingEffectsToRemove == 0)
				break;
		}

		// If we iterated through the entire container and didn't find a match, end here
		if (!hasRemoved)
		{
			break;
		}
	}

}
#if WITH_EDITOR
/// <summary>
/// This is used to make sure that the component is used on the correct kind of gameplay effect
/// (Should not be used on periodic effects)
/// </summary>
/// <param name="Context">The context that will receive the validation data to display in editor</param>
/// <returns>The result of the validation</returns>
EDataValidationResult URemoveSetNumOfEffectsComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (GetOwner()->DurationPolicy != EGameplayEffectDurationType::Instant)
	{
		if (GetOwner()->Period.Value > 0.0f)
		{
			Context.AddError(FText::FormatOrdered(LOCTEXT("PeriodicEffectError", "GE is Periodic. Remove {0} and use TagRequirements (Ongoing) instead."), FText::FromString(GetClass()->GetName())));
			Result = EDataValidationResult::Invalid;
		}
	}

	return Result;
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
