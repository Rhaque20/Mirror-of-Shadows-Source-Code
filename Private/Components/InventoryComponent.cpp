

#include "Components/InventoryComponent.h"

#include "Structs/ArmorDataStruct.h"
#include "Structs/RPGMaterialStruct.h"
#include "SavedGame.h"
#include "CustomGameInstance.h"
#include "GAS/ItemGameplayTags.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    bWantsInitializeComponent = true;
    PhaseNumber = 1;

    // ...
}

void UInventoryComponent::InitializeComponent()
{
    Super::InitializeComponent();

    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance && GameInstance->SaveSystem)
    {
        TScriptInterface<ISavableInterface> Interface(this);
        GameInstance->SaveSystem->RegisterSubsystem(Interface);
    }
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(UpdateUITimer,
        this,
        &UInventoryComponent::UpdateUIDisplays,
        0.2f);
    // ...

}

bool UInventoryComponent::AddArmor(FArmorData ArmorData)
{
    if (Inventory.Num() >= InventoryLimit || ArmorData.EquipmentSlot == FGameplayTag::EmptyTag)
        return false;
    
    FItemSpec Spec = FItemSpec();

    Spec.SetInventoryItem(ArmorData);
    Spec.bCanStack = false;
    Spec.ItemCategory = EItemCategory::Armor;
    Spec.ItemGrade = ArmorData.EquipmentGrade;
    Inventory.Add(FName(ArmorData.ArmorGUID.ToString()),Spec);
    IndexedEquipmentStorage.Add(ArmorData.ArmorGUID, Spec);

    // OnReceiveArmor.Broadcast(ArmorData);
    OnReceiveItem.Broadcast(Spec,1);

    return true;
}

bool UInventoryComponent::AddItemMaterial(FRPGMaterialData Material)
{
    FItemSpec Spec;
    if (Inventory.Contains((Material.ItemName)))
    {
        bool bFound = false;
        int index = 0;
        
        Spec = Inventory[Material.ItemName];
        
        /*Spec = Inventory.Get(IndexedItemStorage[Material.ItemName])
        for (FItemSpec spec : Inventory)
        {
            if (spec.ItemCategory == EItemCategory::CraftingMaterial)
            {
                if (spec.ItemName == Material.ItemName)
                {
                    Spec = spec;
                    bFound = true;
                    break;
                }
            }
            index++;
        }*/
        

        Spec.Stacks += Material.Amount;
        Spec.ItemGrade = Material.ItemGrade;
        FRPGMaterialData tempItem = *Spec.RetrieveItem<FRPGMaterialData>();
        tempItem.Amount = Spec.Stacks;
        Spec.SetInventoryItem(tempItem);
        
        IndexedItemStorage.Add(Material.ItemName, Spec);
    }
    else
    {
        if (Inventory.Num() >= InventoryLimit)
            return false;

        Spec = FItemSpec();

        Spec.SetInventoryItem(Material);
        Spec.bCanStack = true;
        Spec.ItemCategory = EItemCategory::CraftingMaterial;
        Spec.ItemName = Material.ItemName;
        Spec.Stacks = Material.Amount;
        Spec.ItemGrade = Material.ItemGrade;
        
        IndexedItemStorage.Add(Spec.ItemName, Spec);
    }
    
    Inventory.Add(Material.ItemName,Spec);
    OnReceiveMaterial.Broadcast(*Inventory[Material.ItemName].RetrieveItem<FRPGMaterialData>());
    OnReceiveItem.Broadcast(Inventory[Material.ItemName],Material.Amount);

    

    return true;

}

FArmorData UInventoryComponent::GetArmorData(FItemSpec ItemSpec)
{
    return ItemSpec.RetrieveArmorData();
}

void UInventoryComponent::ReceiveItemDirectly(FGameplayTag FlagItemTag)
{
    if (FlagItemTag.MatchesTag(TAG_Item_Recipe))
    {
        if (!RecipesHeld.Contains(FlagItemTag))
        {
            RecipesHeld.Add(FlagItemTag);
            OnReceiveItemDirect.Broadcast(FlagItemTag);
        }
    }
    else if (!FlagItems.Contains(FlagItemTag))
    {
        FlagItems.Add(FlagItemTag);
        OnReceiveItemDirect.Broadcast(FlagItemTag);
    }
}

FRPGMaterialData UInventoryComponent::GetMaterialData(FItemSpec ItemSpec)
{
    return ItemSpec.RetrieveMaterialData();
}

bool UInventoryComponent::RemoveArmor(FArmorData Armor)
{
    FGuid ArmorID = Armor.ArmorGUID;
    FName ArmorName = FName(ArmorID.ToString());
    if (Inventory.Contains(ArmorName))
    {
        FItemSpec Data;
        Inventory.RemoveAndCopyValue(ArmorName,Data);
        OnRemoveItem.Broadcast(Data,1);


        //OnRemoveArmor.Broadcast(Armor);
        return true;
    }

    return false;
}

bool UInventoryComponent::RemoveItem(FGameplayTag ItemToRemove, int Amount)
{
    FName ItemName = ItemToRemove.GetTagName();
    // Item data to manipulate
    int index = -1;

    if (Inventory.Contains(ItemName))
    {
        // Spec from inventory itself
        FItemSpec Spec = Inventory[ItemName];
        
        // Decrement amount taken from spec
        Spec.Stacks -= Amount;
        if (Spec.Stacks <= 0)// If all stacks are cleared
        {
            Inventory.RemoveAndCopyValue(ItemName,Spec);
            OnRemoveItem.Broadcast(Spec,-Amount);
            
                
        }
        else
        {
            FRPGMaterialData ItemData = *Spec.RetrieveItem<FRPGMaterialData>();
            ItemData.Amount = Spec.Stacks;
            
            Spec.SetInventoryItem(ItemData);
            Inventory[ItemName].SetInventoryItem(ItemData);
            Inventory[ItemName].Stacks = ItemData.Amount;
            
            OnReceiveItem.Broadcast(Spec,Amount);
        }
        return true;
    }

    return false;
}

bool UInventoryComponent::RemoveMultipleItems(TMap<FGameplayTag, int> ItemsToRemove)
{
    TArray<FGameplayTag> Keys;
    FName ItemName;
    FRPGMaterialData ItemDummy;

    ItemsToRemove.GenerateKeyArray(Keys);

    for (FGameplayTag tag : Keys)
    {
        ItemName = tag.GetTagName();
        if (!HasSufficientItemsInInventory(ItemName,ItemDummy,ItemsToRemove[tag]))
        {
            return false;
        }
    }

    for (FGameplayTag tag : Keys)
    {
        RemoveItem(tag,ItemsToRemove[tag]);
    }
    

    return true;
}

bool UInventoryComponent::HasSufficientItemsInInventory(FName itemName,FRPGMaterialData& Item,int Amount) const
{
    if (Inventory.Contains(itemName))
    {
        Item = *Inventory[itemName].RetrieveItem<FRPGMaterialData>();
        return Inventory[itemName].Stacks >= Amount;
    }

    return false;
}

int32 UInventoryComponent::GetSavePhase() const
{
    return PhaseNumber;
}

void UInventoryComponent::SaveData(USavedGame& Root)
{
    FInventoryToSave SavedInventory;
    SavedInventory.ItemsToSave = Inventory;
    SavedInventory.RecipesHeld = RecipesHeld;
    SavedInventory.KeyItems = FlagItems;

    /*SavedInventory.SavedArmor = ArmorDataToSave;*/
    Root.Inventory = SavedInventory;
}

void UInventoryComponent::LoadData(const USavedGame& Root)
{
    if (Inventory.Num() > 0)
        return;

    Inventory = Root.Inventory.ItemsToSave;
}

void UInventoryComponent::InitializeData()
{
    
}

void UInventoryComponent::UpdateUIDisplays()
{
    TArray<FName> ItemNames;
    Inventory.GetKeys(ItemNames);
    for (FName specName : ItemNames)
    {
        // if (spec.ItemCategory == EItemCategory::Armor)
        // {
        //     OnReceiveArmor.Broadcast(*spec.RetrieveItem<FArmorData>());
        // }
        // else if (spec.ItemCategory == EItemCategory::CraftingMaterial)
        // {
        //     OnReceiveMaterial.Broadcast(*spec.RetrieveItem<FRPGMaterialData>());
        // }
        
        FItemSpec spec = Inventory[specName];

        OnReceiveItem.Broadcast(spec,spec.Stacks);
    }
}
