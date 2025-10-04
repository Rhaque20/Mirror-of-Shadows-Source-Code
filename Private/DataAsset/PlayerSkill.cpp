// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/PlayerSkill.h"

void UPlayerSkill::FillSkillEnergy(float EnergyGain)
{
	if (!IsReadyToUse() && bHasNRGCost && bCanGainSkillEnergy)
	{
		UE_LOG(LogTemp, Display, TEXT("%s gained %f energy"), *(GetName()), EnergyGain);
		CurrentEnergy += EnergyGain;
	}
}

void UPlayerSkill::ExpendSkillEnergy(float EnergyUseRatio)
{
	if (IsReadyToUse())
	{
		CurrentEnergy = (1.0 - EnergyUseRatio) * EnergyCost;
	}
}
