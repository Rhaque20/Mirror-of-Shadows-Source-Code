// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Structs/ArmorDataStruct.h"
#include "Structs/RPGMaterialStruct.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddArmor(FArmorData ArmorData)
{
	if (Inventory.Num() >= InventoryLimit)
		return false;
	FItemSpec Spec = FItemSpec();

	Spec.SetInventoryItem(ArmorData);
	Spec.bCanStack = false;
	Spec.ItemCategory = EItemCategory::Armor;
	Inventory.Add(Spec);
	IndexedEquipmentStorage.Add(ArmorData.ArmorGUID,Spec);

	return true;
}

bool UInventoryComponent::AddItemMaterial(FRPGMaterialData Material)
{
	FItemSpec Spec;
	if (IndexedItemStorage.Contains((Material.ItemName)))
	{
		FItemSpec tempSpec = IndexedItemStorage[Material.ItemName];
		int index = Inventory.Find(tempSpec);
		Spec = Inventory[index];
		Spec.Stacks++;
		IndexedItemStorage.Add(Material.ItemName,IndexedItemStorage[Material.ItemName]);
	}
	else
	{
		if (Inventory.Num() >= InventoryLimit)
			return false;
		
		Spec = FItemSpec();

		Spec.SetInventoryItem(Material);
		Spec.bCanStack = false;
		Spec.ItemCategory = EItemCategory::CraftingMaterial;
		Spec.ItemName = Material.ItemName;
		
		Inventory.Add(Spec);
		IndexedItemStorage.Add(Spec.ItemName,Spec);
	}

	return true;
	
}

FArmorData UInventoryComponent::GetArmorData(FItemSpec ItemSpec)
{
	if (ItemSpec.ItemCategory == EItemCategory::Armor)
	{
		return *ItemSpec.RetrieveItem<FArmorData>();
	}
	return FArmorData();
}

bool UInventoryComponent::RemoveArmor(FArmorData Armor)
{
	FGuid ArmorID = Armor.ArmorGUID;
	if (IndexedEquipmentStorage.Contains(ArmorID))
	{
		FItemSpec Data = IndexedEquipmentStorage[ArmorID];
		IndexedEquipmentStorage.Remove(ArmorID);
		int i;
		for (i = 0; i < Inventory.Num();i++)
		{
			if (Inventory[i].ItemCategory == EItemCategory::Armor)
			{
				if (*Inventory[i].RetrieveItem<FArmorData>() == Armor)
					break;
			}
		}

		if (i < Inventory.Num())
		{
			Inventory.RemoveAt(i);
			OnRemoveArmor.Broadcast(Armor);
			return true;
		}
	}

	return false;
}

