// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "EntityRewardComponent.generated.h"

USTRUCT(BlueprintType)
struct FItemDropData
{
	GENERATED_BODY()
	public:
		UPROPERTY(BlueprintReadOnly,EditAnywhere)
		FGameplayTag ItemTag;
		UPROPERTY(BlueprintReadOnly,EditAnywhere)
		bool bHasVariableDropAmount = false;
		UPROPERTY(BlueprintReadOnly,EditAnywhere)
		bool bInDropPool = false;
		UPROPERTY(BlueprintReadOnly,EditAnywhere,meta = (EditCondition = "bInDropPool",EditConditionHides))
		int DropPoolID = 0;
		UPROPERTY(BlueprintReadOnly,EditAnywhere,meta = (ClampMin = 1,EditCondition = "bInDropPool",EditConditionHides))
		float DropWeight;
		UPROPERTY(BlueprintReadOnly,EditAnywhere, meta=(ClampMin = 0.00001, ClampMax = 100, EditCondition="!bInDropPool",EditConditionHides))
		float DropPercent = 100.0f;
		UPROPERTY(BlueprintReadOnly,EditAnywhere,meta = (EditCondition = "bHasVariableDropAmount",EditConditionHides))
		FVector2D DropRange;
		UPROPERTY(BlueprintReadOnly,EditAnywhere,meta = (EditCondition = "!bHasVariableDropAmount",EditConditionHides))
		int DropAmount = 1;
	public:
		// Return the amount that would be dropped, either fixed or in a range
		int ReturnDropAmount()
		{
			return bHasVariableDropAmount ? FMath::RandRange((int)DropRange.X, (int)DropRange.Y) : DropAmount;
		}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable )
class MIRROROFSHADOWS_API UEntityRewardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEntityRewardComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	TMap<FName,int> RollForRewards();

	UFUNCTION(BlueprintCallable)
	float RewardEXP(int level);

protected:
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TArray<FItemDropData> ItemDrops;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	bool bUseCustomEXPValue = false;

	UPROPERTY(BlueprintReadOnly,EditAnywhere, meta = (EditCondition = "bUseCustomEXPValue",EditConditionHides))
	float CustomExpValue = 0;

	UPROPERTY(BlueprintReadOnly,EditAnywhere, meta = (EditCondition = "!bUseCustomEXPValue",EditConditionHides))
	UCurveFloat* EXPScaleCurve;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FCurveTableRowHandle LevelEXPCurve;
	
		
};
