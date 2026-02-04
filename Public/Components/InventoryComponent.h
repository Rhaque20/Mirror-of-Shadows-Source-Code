
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/RPGItemStruct.h"
#include "InstancedStruct.h"
#include "Structs/ItemSpec.h"
#include "Structs/ArmorDataStruct.h"
#include "Structs/StructToSave.h"
#include "Interfaces/ISavableInterface.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveMaterial, FRPGMaterialData, RemovedMaterial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveMaterial, FRPGMaterialData, ReceivedMaterial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReceiveItem, FItemSpec, ReceivedItem, int, AmountReceived);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoveItem, FItemSpec, RemovedItem, int, AmountRemoved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveItemDirect, FGameplayTag, FlagItemTag);
class URPGItem;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class MIRROROFSHADOWS_API UInventoryComponent : public UActorComponent, public ISavableInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInventoryComponent();

    virtual void InitializeComponent() override;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
    int InventoryLimit = 25;
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TMap<FName,FItemSpec> Inventory;
    // Used to index items and keep dupes in same spot
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TMap<FName, FItemSpec> IndexedItemStorage;

    // Used to index gear, namely armor and weapons since easy differing factor will be GUID
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TMap<FGuid, FItemSpec> IndexedEquipmentStorage;
    
    UPROPERTY(BlueprintReadOnly)
    TSet<FGameplayTag> FlagItems;
    
    UPROPERTY(BlueprintReadOnly)
    TSet<FGameplayTag> RecipesHeld;

public:

    UFUNCTION(BlueprintCallable)
    bool AddArmor(FArmorData ArmorData);

    UFUNCTION(BlueprintCallable)
    bool AddItemMaterial(FRPGMaterialData Material);

    UFUNCTION(BlueprintCallable)
    FArmorData GetArmorData(FItemSpec ItemSpec);
    
    UFUNCTION(BlueprintCallable)
    void ReceiveItemDirectly(FGameplayTag FlagItemTag);

    UFUNCTION(BlueprintCallable)
    FRPGMaterialData GetMaterialData(FItemSpec ItemSpec);

    UFUNCTION(BlueprintCallable)
    bool RemoveArmor(FArmorData Armor);

    UFUNCTION(BlueprintCallable)
    bool RemoveItem(FGameplayTag ItemToRemove, int Amount);

    UFUNCTION(BlueprintCallable)
    bool RemoveMultipleItems(TMap<FGameplayTag,int> ItemsToRemove);

    UFUNCTION(BlueprintCallable)
    bool HasSufficientItemsInInventory(FName itemName, FRPGMaterialData& Item,int Amount = -1) const;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnRemoveMaterial OnRemoveMaterial;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnReceiveMaterial OnReceiveMaterial;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnReceiveItem OnReceiveItem;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnRemoveItem OnRemoveItem;
    
    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnReceiveItemDirect OnReceiveItemDirect;

    UFUNCTION(BlueprintCallable)
    bool IsInventoryFull() const
    {
        return Inventory.Num() > InventoryLimit;
    }
    

    virtual int32 GetSavePhase() const override;
    virtual void SaveData(USavedGame& Root) override;
    virtual void LoadData(const USavedGame& Root) override;
    virtual void InitializeData() override;

private:
    FTimerHandle UpdateUITimer;

    int32 PhaseNumber;
    UFUNCTION()
    void UpdateUIDisplays();

};
