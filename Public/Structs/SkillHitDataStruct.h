#include "GameplayTagContainer.h"
#include "HitScanShape.h"
#include "ParryHitStruct.h"
#include "GAS/UDGameplayTags.h"
#include "SkillHitDataStruct.generated.h"
#pragma once

USTRUCT(BlueprintType)
struct FSkillHitData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText HitSequenceName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SkillModifier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoiseDamage = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag HitElement = TAG_Damage_Health_Physical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackPower = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasVerticalLaunch = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* AdditionalHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bHasVerticalLaunch", EditConditionHides))
	float VerticalLaunchPower = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StunDamageModifier = 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UAkAudioEvent* HitSoundEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HitBoxLocation = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitScanShape HitScanShapeData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HitBoxScale = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FParryHitType HitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HitLimit = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttachedToSocket = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bAttachedToSocket", EditConditionHides))
	FName SocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bAttachedToSocket", EditConditionHides))
	FName EndSocketName;

};