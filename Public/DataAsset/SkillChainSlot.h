// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enumerator/EffectTargetType.h"
#include "SkillChainSlot.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MIRROROFSHADOWS_API USkillChainSlot : public UDataAsset
{
	GENERATED_BODY()

	protected:
		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
		TArray<float> SPCostIncreaseMod;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TSubclassOf<class UGameplayEffect> Effect;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		EEffectTargetType EffectTarget;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Icon")
		class UPaperSprite* SlotEffectIcon;


		
};
