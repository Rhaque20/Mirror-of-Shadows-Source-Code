// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaggerComponent.h"
#include "RPGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/UDGameplayTags.h"

// Sets default values for this component's properties
UStaggerComponent::UStaggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStaggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStaggerComponent::ApplyKnockback(FVector AttackerLocation, FVector2D KnockbackPower, bool bExtendAirTime)
{
	ARPGCharacterBase* Character = Cast<ARPGCharacterBase>(GetOwner());

	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();

	if (!IsValid(ASC))
		return;

	if (ASC->HasMatchingGameplayTag(TAG_Effect_State_PoiseBroken) ||
		ASC->HasMatchingGameplayTag(TAG_Effect_State_Stagger))
	{
		FVector KnockbackDirection = (Character->GetActorLocation() - AttackerLocation);

		KnockbackDirection.Normalize();

		KnockbackDirection = FVector(KnockbackDirection.X * KnockbackPower.X, KnockbackDirection.Y * KnockbackPower.X, KnockbackPower.Y);

		if (!(Character->IsOnGround()))
		{
			if (KnockbackPower.Y == 0.0f)
			{
				if (bExtendAirTime)
				{
					Character->TriggerAirTime();
					UE_LOG(LogTemp, Display, TEXT("StaggerComponentCPP: Triggering air combos for %s"), *(Character->GetName()));
				}
			}
			else
			{
				Character->EndAirTime();
			}
		}

		Character->LaunchCharacter(KnockbackDirection, false, true);
		/*else
		{
			if (Character->IsOnGround())
			{
				UE_LOG(LogTemp, Display, TEXT("Character %s attacked on ground"), *(Character->GetName()));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Character %s attacked in air"), *(Character->GetName()));
			}
		}*/

		UE_LOG(LogTemp, Display, TEXT("StaggerComponentCPP: causing knockback on enemy with knockback direction %s"),*KnockbackDirection.ToString());
	}
}

