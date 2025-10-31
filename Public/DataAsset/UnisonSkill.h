#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Enumerator/EffectTargetType.h"

#include "UnisonSkill.generated.h"


UCLASS(Blueprintable)
class MIRROROFSHADOWS_API UUnisonSkill : public UDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UPaperSprite* SkillIcon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float UnisonSkillCost = 20.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> UnisonSkillEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayAbility> UnisonSkillAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EEffectTargetType EffectTarget;

public:
	float GetUnisonSkillCost() const
	{
		return UnisonSkillCost;
	}

	TSubclassOf<UGameplayAbility> GetUnisonSkillAbility() const
	{
		return UnisonSkillAbility;
	}

	TSubclassOf<UGameplayEffect> GetUnisonSkillEffect() const
	{
		return UnisonSkillEffect;
	}

	EEffectTargetType GetEffectTargetType() const
	{
		return EffectTarget;
	}

	UPaperSprite* GetSkillIcon() const
	{
		return SkillIcon;
	}
};
