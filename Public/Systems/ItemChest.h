// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Enumerator/ItemCategoryEnum.h"
#include "GameFramework/Actor.h"
#include "Structs/ArmorDataStruct.h"
#include "Interfaces/InteractableInterface.h"
#include "Enumerator/ItemEnum.h"

#include "ItemChest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChestOpen,AItemChest*, OpenedChest);
UCLASS()
class MIRROROFSHADOWS_API AItemChest : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemChest();

	UFUNCTION()
	void UnlockChest(class AEnemySpawnerSystem* DefeatedSpawner);

	UFUNCTION()
	void LockChest();

	UFUNCTION()
	void ChestToggle(bool bSetVisible);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (ToolTip = "For Rarity Ranges, the float values represent probability weight rather than direct percentages"))
	void GenerateArmorsToGive(const TMap<EGrade, float>& RarityRanges, const FGameplayTag& ItemSetTag, const FVector2D& ItemAmountRange);

	bool IsRareChest() const
	{
		return bIsRareChest;
	}

public:
	UPROPERTY(BlueprintCallable)
	FOnChestOpen OnChestOpen;
	
	UPROPERTY(BlueprintReadOnly,EditInstanceOnly, Category = "Strongbox Reward", meta = (EditCondition = "InteractiveType == EInteractiveType::OneTime",EditConditionHides))
	EItemCategory StrongBoxRewardCategory;
	
	UPROPERTY(BlueprintReadOnly,EditInstanceOnly,Category = "Strongbox Reward",meta = (EditCondition = "InteractiveType == EInteractiveType::OneTime && StrongBoxRewardCategory == EItemCategory::CraftingRecipe",EditConditionHides))
	FGameplayTag RecipeTag;
	

protected:

	UPROPERTY(BlueprintReadOnly)
	bool bIsLocked = false;

	UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
	bool bIsRareChest = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<FArmorData> ArmorsToGive;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	class UNiagaraComponent* LockedVFX;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	class UStaticMeshComponent* ChestMesh;

	USceneComponent* TestComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
