// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"
#include "AbilitySystemComponent.h"
#include "AsyncTaskEffectStackChanged.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGameplayEffectStackChanged, FGameplayTag, EffectGameplayTag, FActiveGameplayEffectHandle, Handle, int32, NewStackCount, int32, OldStackCount);
DECLARE_MULTICAST_DELEGATE_FourParams(FOnGameplayEffectStackChangeCPP, FGameplayTag, FActiveGameplayEffectHandle,int32, int32);
DECLARE_DELEGATE_FourParams(FStackChangeFunction, FGameplayTag, FActiveGameplayEffectHandle, int32, int32);


/**
 * Blueprint node to automatically register a listener for changes to a GameplayEffect's stack count based on an Asset or Granted tag on the Effect.
 * Useful to use in UI.
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class MIRROROFSHADOWS_API UAsyncTaskEffectStackChanged : public UAbilityAsync
{
	GENERATED_BODY()
	
	public:
		UPROPERTY(BlueprintAssignable)
		FOnGameplayEffectStackChanged OnGameplayEffectStackChange;

		FOnGameplayEffectStackChangeCPP OnGameplayEffectStackChangeCPP;

	public:
		static UAsyncTaskEffectStackChanged* ListenForGameplayEffectStackChange_CPP(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag EffectGameplayTag, bool ExactMatch, FStackChangeFunction&& FunctionToBind);

	private:
		UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UAsyncTaskEffectStackChanged* ListenForGameplayEffectStackChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag EffectGameplayTag, bool ExactMatch);

	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	FGameplayTag EffectGameplayTag;

	FActiveGameplayEffectHandle ActiveEffectHandle;

	bool bListenForExactMatch = false;

	int OldStacks = 0, NewStacks = 0;

	FGameplayTag ReturnTagBasedOnContainer(TArray<FGameplayTag> TagContainer);

	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);

	virtual void GameplayEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount);
};
