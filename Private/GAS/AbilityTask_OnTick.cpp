#include "GAS/AbilityTask_OnTick.h"

UAbilityTask_OnTick::UAbilityTask_OnTick(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

UAbilityTask_OnTick* UAbilityTask_OnTick::AbilityTaskOnTick(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UAbilityTask_OnTick* NewTask = NewAbilityTask<UAbilityTask_OnTick>(OwningAbility);
	return NewTask;
}

void UAbilityTask_OnTick::Activate()
{
	Super::Activate();
}

void UAbilityTask_OnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(DeltaTime);
	}
}


