// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/ISavableInterface.h"
#include "CharacterGrowthComponent.generated.h"

class USkillTreeNodeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUp, APlayerCharacters*, LeveledUpActor,
											 int, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnXPChange, APlayerCharacters*, LevelingUpActor,
											 float, CurrentXP, float, XPToNextLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAPChange,
											 float, CurrentAP, float, MaxAP);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRROROFSHADOWS_API UCharacterGrowthComponent : public UActorComponent, public ISavableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterGrowthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float TotalXP = 0.0f;
	UPROPERTY()
	float TotalAP = 0.0f;
	UPROPERTY()
	float TotalXPRequired = 0.0f;
	UPROPERTY()
	float NextLevelXPRequired = 0.0f;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int CharacterLevel = 1;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, meta=(ToolTip="Amount of XP required ot the next level"))
	FCurveTableRowHandle XPRequirementRow;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,meta=(ToolTip="Amount of XP accumulated at that level"))
	FCurveTableRowHandle BaseTotalXPRow;

	UPROPERTY()
	class APlayerCharacters* PlayerOwner;
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystem;
	
	UPROPERTY()
	TArray<USkillTreeNodeData*> ActiveSkillTreeNodes;
	UPROPERTY()
	TArray<FName> ActiveSkillTreeNodeNames;

	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnLevelUp OnLevelUp;

	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnXPChange OnXPChange;

	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnAPChange OnAPChange;

public:	
	void ReceiveXP(float BaseXPAmount);
	float ReturnXPAmount() const
	{
		return TotalXP;
	}

	UFUNCTION(BlueprintCallable)
	void DeductAP(float APCost);

	void ReceiveSkillTreeNode(USkillTreeNodeData* SkillTreeNode, FName SkillID);

public:
    virtual void InitializeComponent() override;
    virtual int32 GetSavePhase() const override;
    virtual void SaveData(USavedGame& Root) override;
    virtual void LoadData(const USavedGame& Root) override;
	virtual void InitializeData() override;

private:

    int32 PhaseNumber;
};
