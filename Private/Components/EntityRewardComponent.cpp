// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EntityRewardComponent.h"
#include <vector>

// Sets default values for this component's properties
UEntityRewardComponent::UEntityRewardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEntityRewardComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

TMap<FName, int> UEntityRewardComponent::RollForRewards()
{
	TMap<FName,int> Rewards;

	// Pool of pools
	std::vector<std::vector<FItemDropData>> DropPools;
	// Used to keep track of the total weight for each pool
	TArray<float> TotalWeights;
	float rngNumber = 0.0f;

	for (FItemDropData ItemData: ItemDrops)
	{
		if (ItemData.bInDropPool)
		{
			// Check to make sure we have that pool in our pool of pools
			if (DropPools.size() < ItemData.DropPoolID+1)
			{
				while (DropPools.size() < ItemData.DropPoolID+1)
				{
					DropPools.push_back(std::vector<FItemDropData>());
					TotalWeights.Add(0.0f);
				}
			}

			// Put that item into its respective pool and increase the total weight of that pool
			DropPools[ItemData.DropPoolID].push_back(ItemData);
			TotalWeights[ItemData.DropPoolID] += ItemData.DropWeight;
		}
		else
		{
			// If not in a pool, random roll between 1 and 100 and determine if obtained or not.
			rngNumber = FMath::RandRange(1.0f,100.0f);
			if (rngNumber < ItemData.DropPercent)
			{
				Rewards.Add(ItemData.ItemTag.GetTagName(),ItemData.ReturnDropAmount());
			}
		}
	}

	if (DropPools.size() > 0)
	{
		std::vector<FItemDropData> CurrentPool;
		float currentWeight = 0.0f;
		// Start going through each pool and pull one item out of each pool.
		while (DropPools.size() > 0)
		{
			CurrentPool = *DropPools.begin();
			rngNumber = FMath::RandRange(1.0f,*TotalWeights.begin());
			
			for (FItemDropData item: CurrentPool)
			{
				// If random value is GREATER than the current weight AND its drop weight,
				// Move onto the next object and increase the weight that it gone through.
				if (rngNumber > currentWeight + item.DropWeight)
				{
					currentWeight += item.DropWeight;
				}
				else
				{
					// Else, grab the item from the pool and stop going further down the pool.
					Rewards.Add(item.ItemTag.GetTagName(),item.ReturnDropAmount());
					break;
				}
			}
			// Remove the current pool
			DropPools.erase(DropPools.begin());
			// Remove that pool's total weight
			TotalWeights.RemoveAt(0);
			// And reset
			currentWeight = 0.0f;
		}
		
	}

	return Rewards;
}

float UEntityRewardComponent::RewardEXP(int level)
{
	if (bUseCustomEXPValue)
		return CustomExpValue;

	float FinalEXP = 0.0f;

	float BaseEXP = LevelEXPCurve.Eval(level,FString("EXP"));

	FinalEXP = BaseEXP * EXPScaleCurve->GetFloatValue(level);
	
	return FinalEXP;
}

