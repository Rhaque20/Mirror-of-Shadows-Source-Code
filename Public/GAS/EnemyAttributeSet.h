// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAttributeSet.h"
#include "EnemyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class MIRROROFSHADOWS_API UEnemyAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()

	public:
		UPROPERTY(BlueprintReadOnly, Category = "Meta")
		FGameplayAttributeData StunDMG;
		BASEGAS_ATTRIBUTE_ACCESSORS(UEnemyAttributeSet,StunDMG);

		UPROPERTY(BlueprintReadOnly, Category = "Current Stun")
		FGameplayAttributeData CurrentStun;
		BASEGAS_ATTRIBUTE_ACCESSORS(UEnemyAttributeSet,CurrentStun);

		UPROPERTY(BlueprintReadOnly, Category = "Max Stun")
		FGameplayAttributeData MaxStun;
		BASEGAS_ATTRIBUTE_ACCESSORS(UEnemyAttributeSet,MaxStun);

		UPROPERTY(BlueprintReadOnly, Category = "Stun Resistance")
		FGameplayAttributeData StunRes;
		BASEGAS_ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, StunRes);

	public:
		virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
		// virtual void HandleEvaluatedData(const FGameplayEffectModCallbackData& Data, bool IsPostEffect);
		/*virtual void PoiseBreak(const FGameplayEffectModCallbackData& Data) override;*/

		void StanceBreak(const FGameplayEffectModCallbackData& Data);
	
};
