// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusEffectComponent.h"
#include "GAS/UDGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"



// Sets default values for this component's properties
UStatusEffectComponent::UStatusEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	IAbilitySystemInterface* ImplementedOwner = Cast<IAbilitySystemInterface>(GetOwner());
	if (ImplementedOwner)
	{
		OwnerAbilitySystem = ImplementedOwner->GetAbilitySystemComponent();
		StatusDecayTimeRemainingMap = TMap<EStatusEffect, float>();
	}
	// ...
	
}

void UStatusEffectComponent::ApplyStatusDamage(FGameplayTag StatusTag, float StatusDamage)
{
	if (OwnerAbilitySystem)
	{
		UE_LOG(LogTemp, Display, TEXT("Ability System is valid"));
		FGameplayTag ActiveEffectTag = StatusTag;
		EStatusEffect StatusID = EStatusEffect::Burn;

		// To be used when handling DoT where it can continue to build up even after the effect is active
		// for stacking.
		bool bCanOverrideforDoT = false;

		// Series of ifs cause cannot switch with tags to determine which tags to use.
		// And what enums to use as well.
		if (StatusTag == TAG_Damage_Status_Silence)
		{
			UE_LOG(LogTemp, Display, TEXT("Silence Damage received %f"),StatusDamage);
			ActiveEffectTag = TAG_Status_Ailment_Silence;
			StatusID = EStatusEffect::Silence;
		}
		else if (StatusTag == TAG_Damage_Status_Slow)
		{
			ActiveEffectTag = TAG_Status_Ailment_Slow;
			StatusID = EStatusEffect::Slow;
		}
		else
			return;

		AActor* Owner = GetOwner();

		// Check if the status is already active. Do not build up if the status is active
		// With the boolean, this can be bypassed, but not implemented yet.
		if (!OwnerAbilitySystem->HasMatchingGameplayTag(ActiveEffectTag))
		{
			// Accumulate the status damage
			StatusDamageAccumulation[(int)StatusID] += StatusDamage;
			UE_LOG(LogTemp, Display, TEXT("Status Damage accumulated"));

			// If accumulation reaches max, grant the status effect.
			if (StatusDamageAccumulation[(int)StatusID] >= 100.0f)
			{
				// Resets the accumulation
				StatusDamageAccumulation[(int)StatusID] = 0.0f;

				// Removes it from the decay map as to prevent unnecessary decay ticking 
				// on an active status
				StatusDecayTimeRemainingMap.Remove(StatusID);
				// Select which status to apply based on the previously stored
				// status ID.
				TSubclassOf<UGameplayEffect> StatusClass = nullptr;

				switch (StatusID)
				{
					case EStatusEffect::Silence:
						StatusClass = SilenceStatus;
						break;
					case EStatusEffect::Slow:
						StatusClass = SlowStatus;
						break;
				}

				// If it's a valid status class, apply the effect.
				if (StatusClass)
				{
					FGameplayEffectContextHandle context = OwnerAbilitySystem->MakeEffectContext();
					OwnerAbilitySystem->BP_ApplyGameplayEffectToSelf(StatusClass, 1, context);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("EFFECT CLASS POINTER IS NULL AT %s"), *(Owner->GetName()));
					return;
				}

			}
			else // If accumulation has not capped, add/refresh the decay timer of that respective status.
			{
				AddDecayTimer(StatusID);
			}
			// Broadcast the build up
			BroadcastStatusBuildUp(StatusDamageAccumulation[(int)StatusID], StatusTag);
		}
		else if (bCanOverrideforDoT)
		{
			// Do build up here and apply effect if build up is reached before cyclicly restarting
		}
	}
}

void UStatusEffectComponent::StatusBuildUpDecayTick()
{
	// Get all the keys of the statuses in possession.
	TArray<EStatusEffect> KeyArray;
	StatusDecayTimeRemainingMap.GenerateKeyArray(KeyArray);
	// If there are no keys to read, end this tick function.
	if (KeyArray.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(StatusBuildUpDecayTimer);
		return;
	}

	// Used to handle deletion post-traversal
	TArray<EStatusEffect> MarkedForDeletion;
	float remainingTime;

	// Used for broadcasting the status that has its accumulation changed.
	FGameplayTag StatusTag;
	for(EStatusEffect effectKey : KeyArray)
	{
		// Get the remaining time of the status based on the passed in key arrays.
		// Null pointer reading should be non-issue due to the keys being used are ones retrieved
		// by the map.
		remainingTime = *(StatusDecayTimeRemainingMap.Find(effectKey));
		
		// If it still has time remaining before decay, decrement the remaining time
		if (remainingTime > 0.0f)
		{
			remainingTime -= DecayInterval;
			StatusDecayTimeRemainingMap.Add(effectKey, remainingTime);
		}
		else // Else decay the accummulation
		{
			// In the event the interval were to ever change, make it will scale the
			// DPS (Decay Per Second) by the actual interval similar to deltaTIme.
			StatusDamageAccumulation[(int)effectKey] -= DecayPerSecond * DecayInterval;

			// Grab the status damage tag (not actual status) based on the key.
			// This is because the widget stores the status damage widgets in a map
			// Using the status damage type as the key.
			switch (effectKey)
			{
				case EStatusEffect::Silence:
					StatusTag = TAG_Damage_Status_Silence;
					break;
				case EStatusEffect::Slow:
					StatusTag = TAG_Damage_Status_Slow;
					break;
			}

			// Mark for deletion if the accumulation is empty from the decay.
			if (StatusDamageAccumulation[(int)effectKey] <= 0.0f)
			{
				MarkedForDeletion.Add(effectKey);
			}

			BroadcastStatusBuildUp(StatusDamageAccumulation[(int)effectKey], StatusTag);
		}
	}

	// Remove all the map items that has their remaining time <= 0.
	for (EStatusEffect effectKey : MarkedForDeletion)
	{
		StatusDecayTimeRemainingMap.Remove(effectKey);
	}
}

void UStatusEffectComponent::AddDecayTimer(EStatusEffect StatusEnum)
{

	// If no timer is present, make a new one and store it in the handle
	if (!StatusBuildUpDecayTimer.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(
			StatusBuildUpDecayTimer, // handle to cancel timer at a later time
			this, // the owning object
			&UStatusEffectComponent::StatusBuildUpDecayTick, // function to call on elapsed
			DecayInterval, // float delay until elapsed
			true);

	}

	StatusDecayTimeRemainingMap.Add(StatusEnum, TimeUntilDecay);
}

void UStatusEffectComponent::BroadcastStatusBuildUp(float BuildUpAmount, FGameplayTag StatusTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = StatusTag; // Not necessary but nice to put.
	Payload.EventMagnitude = BuildUpAmount/100.0f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), StatusTag, Payload);
}


//// Called every frame
//void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

