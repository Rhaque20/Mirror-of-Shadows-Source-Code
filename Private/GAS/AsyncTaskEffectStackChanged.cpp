// Copyright 2020 Dan Kestranek.


#include "GAS/AsyncTaskEffectStackChanged.h"

UAsyncTaskEffectStackChanged * UAsyncTaskEffectStackChanged::ListenForGameplayEffectStackChange(UAbilitySystemComponent * AbilitySystemComponent, FGameplayTag InEffectGameplayTag, bool ExactMatch)
{
	UAsyncTaskEffectStackChanged* ListenForGameplayEffectStackChange = NewObject<UAsyncTaskEffectStackChanged>();
	ListenForGameplayEffectStackChange->EffectGameplayTag = InEffectGameplayTag;
	ListenForGameplayEffectStackChange->bListenForExactMatch = ExactMatch;
	ListenForGameplayEffectStackChange->SetAbilitySystemComponent(AbilitySystemComponent);

	if (!IsValid(AbilitySystemComponent) || !InEffectGameplayTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("Gameplay tag or ability system component is invalid"));
		ListenForGameplayEffectStackChange->EndTask();
		return nullptr;
	}

	UE_LOG(LogTemp, Display, TEXT("Ability system and effect gameplay tag are valid"));

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(ListenForGameplayEffectStackChange, &UAsyncTaskEffectStackChanged::OnActiveGameplayEffectAddedCallback);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(ListenForGameplayEffectStackChange, &UAsyncTaskEffectStackChanged::OnRemoveGameplayEffectCallback);

	return ListenForGameplayEffectStackChange;
}

void UAsyncTaskEffectStackChanged::EndTask()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		ASC->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
		
		if(ActiveEffectHandle.IsValid())
		{
			ASC->OnGameplayEffectStackChangeDelegate(ActiveEffectHandle)->RemoveAll(this);
		}
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

FGameplayTag UAsyncTaskEffectStackChanged::ReturnTagBasedOnContainer(TArray<FGameplayTag> TagContainer)
{
	if (TagContainer.Num() >= 1)
	{
		return TagContainer[0];
	}
	else
		return EffectGameplayTag;
}

void UAsyncTaskEffectStackChanged::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent * Target, const FGameplayEffectSpec & SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	if (!ShouldBroadcastDelegates())
	{
		return;
	}

	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();


	TArray<FGameplayTag> TagContainer;
	AssetTags.GetGameplayTagArray(TagContainer);


	if (bListenForExactMatch)
	{
		if (AssetTags.HasTagExact(EffectGameplayTag) || GrantedTags.HasTagExact(EffectGameplayTag))
		{
			UE_LOG(LogTemp, Display, TEXT("Effect being increased %s"), *(ReturnTagBasedOnContainer(TagContainer).GetTagName().ToString()));
			ASC->OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddUObject(this, &UAsyncTaskEffectStackChanged::GameplayEffectStackChanged);
			OnGameplayEffectStackChange.Broadcast(ReturnTagBasedOnContainer(TagContainer), ActiveHandle, 1, 0);
			ActiveEffectHandle = ActiveHandle;
		}
	}
	else
	{
		if (AssetTags.HasTag(EffectGameplayTag) || GrantedTags.HasTag(EffectGameplayTag))
		{
			UE_LOG(LogTemp, Display, TEXT("Effect being increased %s"), *(ReturnTagBasedOnContainer(TagContainer).GetTagName().ToString()));
			ASC->OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddUObject(this, &UAsyncTaskEffectStackChanged::GameplayEffectStackChanged);
			OnGameplayEffectStackChange.Broadcast(ReturnTagBasedOnContainer(TagContainer), ActiveHandle, 1, 0);
			ActiveEffectHandle = ActiveHandle;
		}
	}
	
}

void UAsyncTaskEffectStackChanged::OnRemoveGameplayEffectCallback(const FActiveGameplayEffect & EffectRemoved)
{
	FGameplayTagContainer AssetTags;
	EffectRemoved.Spec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectRemoved.Spec.GetAllGrantedTags(GrantedTags);

	TArray<FGameplayTag> TagContainer;
	AssetTags.GetGameplayTagArray(TagContainer);


	if (bListenForExactMatch)
	{
		if (AssetTags.HasTagExact(EffectGameplayTag) || GrantedTags.HasTagExact(EffectGameplayTag))
		{
			UE_LOG(LogTemp, Display, TEXT("Effect being removed %s"), *(ReturnTagBasedOnContainer(TagContainer).GetTagName().ToString()));
			OnGameplayEffectStackChange.Broadcast(ReturnTagBasedOnContainer(TagContainer), EffectRemoved.Handle, 0, 1);
		}
	}
	else
	{
		if (AssetTags.HasTag(EffectGameplayTag) || GrantedTags.HasTag(EffectGameplayTag))
		{
			UE_LOG(LogTemp, Display, TEXT("Effect being removed %s"), *(ReturnTagBasedOnContainer(TagContainer).GetTagName().ToString()));
			OnGameplayEffectStackChange.Broadcast(ReturnTagBasedOnContainer(TagContainer), EffectRemoved.Handle, 0, 1);
		}
	}
}

void UAsyncTaskEffectStackChanged::GameplayEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount)
{
	UE_LOG(LogTemp, Display, TEXT("Stack changed!"));
	OnGameplayEffectStackChange.Broadcast(EffectGameplayTag, EffectHandle, NewStackCount, PreviousStackCount);
}
