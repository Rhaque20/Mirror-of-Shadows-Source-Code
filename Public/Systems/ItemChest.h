// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/ArmorDataStruct.h"
#include "Interfaces/InteractableInterface.h"
#include "Enumerator/ItemEnum.h"

#include "ItemChest.generated.h"

UCLASS()
class MIRROROFSHADOWS_API AItemChest : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemChest();

	UFUNCTION()
	void UnlockChest();

	UFUNCTION()
	void LockChest();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (ToolTip = "For Rarity Ranges, the float values represent probability weight rather than direct percentages"))
	void GenerateArmorsToGive(const TMap<EGrade, float>& RarityRanges, const FGameplayTag& ItemSetTag, const FVector2D& ItemAmountRange);

protected:

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	bool bIsLocked = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<FArmorData> ArmorsToGive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
