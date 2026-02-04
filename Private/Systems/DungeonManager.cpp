
#include "Systems/DungeonManager.h"
#include "GAS/UDGameplayTags.h"
#include "Systems/EnemySpawnerSystem.h"
#include "PlayerPartyController.h"
#include "Blueprint/UserWidget.h"
#include "Systems/ItemChest.h"
#include "CustomGameInstance.h"
#include "GameplayModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADungeonManager::ADungeonManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    PhaseNumber = 3;
}

// Called when the game starts or when spawned
void ADungeonManager::BeginPlay()
{
    Super::BeginPlay();
    
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance && GameInstance->SaveSystem)
    {
        TScriptInterface<ISavableInterface> Interface(this);
        GameInstance->SaveSystem->RegisterSubsystem(Interface);
    }

}

void ADungeonManager::OnDungeonClear(AEnemySpawnerSystem* DefeatedSpawner)
{
    UE_LOG(LogTemp, Display, TEXT("YOU WIN!"));
    OnVictory.Broadcast();

    if (VictoryWidget)
    {
        APlayerController* Controller = GetWorld()->GetFirstPlayerController();
        CreateWidget(GetWorld(), VictoryWidget)->AddToViewport();
        Controller->bShowMouseCursor = true;
    }
    
}

void ADungeonManager::OnSpawnerCleared(class AEnemySpawnerSystem* DefeatedSpawner)
{
    EnemyGroupsCleared.Add(DefeatedSpawner->GetActorGuid());
}

void ADungeonManager::OnDungeonFail()
{
    if (GameOverWidget)
    {
        APlayerController* Controller = GetWorld()->GetFirstPlayerController();
        CreateWidget(GetWorld(), GameOverWidget)->AddToViewport();
    }
}

void ADungeonManager::InitializeData()
{
    if (ItemChestsReferences.Num() == 0)
        UE_LOG(LogTemp,Error,TEXT("Oh shit, we have no chest references"));

    for (int i = 0; i < ItemChestsReferences.Num(); i++)
    {
        if (ChestsClaimed.Contains(ItemChestsReferences[i]->GetActorGuid()))
        {
            ItemChestsReferences[i]->ChestToggle(false);
        }
        else
        {
            ItemChestsReferences[i]->OnChestOpen.AddDynamic(this, &ADungeonManager::OnChestClaimed);
            ItemChestsReferences[i]->GenerateArmorsToGive(ItemChestsReferences[i]->IsRareChest() ? Tier1LootGrade : Tier2LootGrade, SetTag, FVector2D(1, 3));   
        }
    }

    for (AEnemySpawnerSystem* Groups : EnemyGroupReferences)
    {
        if (EnemyGroupsCleared.Contains(Groups->GetActorGuid()))
        {
            continue;
        }
        
        Groups->ActivateSpawner(MaxMeleeTickets,MaxRangedTickets);
            
        if (Groups->ActorHasTag(BossTag))
        {
            Groups->OnAllEnemiesDead.AddDynamic(this, &ADungeonManager::OnDungeonClear);
        }
        
        Groups->OnAllEnemiesDead.AddDynamic(this, &ADungeonManager::OnSpawnerCleared);
    }
    
    for (AInteractableActor* Interactables: LockedInteractables)
    {
        if (UnlockedInteractables.Contains(Interactables->GetActorGuid()))
        {
            Interactables->UnlockInteractable();
        }
        else
        {
            Interactables->OnInteractEvent.AddDynamic(this,&ADungeonManager::OnUnlockingInteractable);
        }
    }

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

    if (PlayerController != nullptr)
    {
        BindPlayerControllerForGameOver(PlayerController);
    }
    else
    {
        AGameModeBase* Gamemode = UGameplayStatics::GetGameMode(GetWorld());
        AGameplayModeBase* GameplayMode = Cast<AGameplayModeBase>(Gamemode);
        
        if (GameplayMode)
        {
            GameplayMode->OnPlayerControllerSpawn.AddDynamic(this,&ADungeonManager::BindPlayerControllerForGameOver);
        }
    }
}

void ADungeonManager::BindPlayerControllerForGameOver(APlayerController* PlayerController)
{
    Cast<APlayerPartyController>(PlayerController)->GameOver.AddDynamic(this, &ADungeonManager::OnDungeonFail);
}

void ADungeonManager::OnChestClaimed(AItemChest* ItemChest)
{
    ChestsClaimed.Add(ItemChest->GetActorGuid());
}

void ADungeonManager::OnUnlockingInteractable(class AInteractableActor* Interactables)
{
    if (!UnlockedInteractables.Contains(Interactables->GetActorGuid()))
    {
        UnlockedInteractables.Add(Interactables->GetActorGuid());
    }
}

int32 ADungeonManager::GetSavePhase() const
{
    return PhaseNumber;
}

void ADungeonManager::SaveData(USavedGame& Root)
{
    FDungeonToSave SaveDat;
    SaveDat.SavedChests = ChestsClaimed;
    SaveDat.SavedEnemySpawner = EnemyGroupsCleared;
    SaveDat.ActivatedInteractables = UnlockedInteractables;
    Root.DungeonManager = SaveDat;
}

void ADungeonManager::LoadData(const USavedGame& Root)
{
    ChestsClaimed = Root.DungeonManager.SavedChests;
    EnemyGroupsCleared = Root.DungeonManager.SavedEnemySpawner;
    UnlockedInteractables = Root.DungeonManager.ActivatedInteractables;
    
    bLoadedStuff = true;
    InitializeData();
}


FDungeonToSave ADungeonManager::GetSaveData()
{
    FDungeonToSave SaveDat;
    SaveDat.SavedChests = ChestsClaimed;
    SaveDat.SavedEnemySpawner = EnemyGroupsCleared;

    return SaveDat;
}

/*void ADungeonManager::LoadSavedData(FDungeonToSave& LoadedData)
{
    ChestsClaimed = LoadedData.SavedChests;
    EnemyGroupsCleared = LoadedData.SavedEnemySpawner;
    UnlockedInteractables = LoadedData.ActivatedInteractables;
    bLoadedStuff = true;
    InitializeDungeon();
}*/