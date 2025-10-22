// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/RPGItemStruct.h"
#include "InstancedStruct.h"
#include "Structs/ItemSpec.h"
#include "Structs/ArmorDataStruct.h"

#include "InventoryComponent.generated.h"

class URPGItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class MIRROROFSHADOWS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
protected:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	int InventoryLimit = 25;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TArray<FItemSpec> Inventory;
	// Used to index items and keep dupes in same spot
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TMap<FName, int> IndexedItemStorage;

	// Used to index gear, namely armor and weapons since easy differing factor will be GUID
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TMap<int,FArmorData> IndexedEquipmentStorage;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool AddArmor(FArmorData ArmorData);
	
	UFUNCTION(BlueprintCallable)
	bool AddItemMaterial(FRPGMaterialData Material);

	UFUNCTION(BlueprintCallable)
	FArmorData GetArmorData(FItemSpec ItemSpec);

		
};
