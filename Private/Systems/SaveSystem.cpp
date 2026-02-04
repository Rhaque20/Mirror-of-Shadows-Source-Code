// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/SaveSystem.h"
#include "PlayerPartyController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InventoryComponent.h"
#include "Systems/DungeonManager.h"
#include "PlayerCharacters.h"
#include "Components/EquipmentComponent.h"

void USaveSystem::PostInitProperties()
{
    Super::PostInitProperties();

    if (!SaveFile)
    {
        SaveFile = NewObject<USavedGame>(this, USavedGame::StaticClass());
    }
}

void USaveSystem::RegisterSubsystem(TScriptInterface<ISavableInterface> Subsystem)
{
    Subsystems.Add(Subsystem);
    if (LoadedSave)
        Subsystem->LoadData(*LoadedFile);
    else
    {
        Subsystem->InitializeData();
    }
}

bool USaveSystem::FindSubsystem(TScriptInterface<ISavableInterface> Subsystem)
{
    int32 Index = Subsystems.Find(Subsystem);
    return Index != INDEX_NONE;
}

void USaveSystem::SaveGame()
{

    Subsystems.Sort([](auto A, auto B)
        {
            return A->GetSavePhase() < B->GetSavePhase();
        });

    int32 MaxPhase = 3;

    for (int32 Phase = 0; Phase <= MaxPhase; Phase++)
    {
        for (auto& Sys : Subsystems)
            if (Sys->GetSavePhase() == Phase)
                Sys->PreSave();

        for (auto& Sys : Subsystems)
            if (Sys->GetSavePhase() == Phase)
                Sys->SaveData(*SaveFile);

        for (auto& Sys : Subsystems)
            if (Sys->GetSavePhase() == Phase)
                Sys->PostSave();
    }
    bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveFile, "SAVEDMAN", 0);

    if (!bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to save game!"));
    }
}

void USaveSystem::LoadGame()
{
    if (!LoadedFile) return;

    // Sort subsystems same as save
    Subsystems.Sort([](auto A, auto B)
        {
            return A->GetSavePhase() < B->GetSavePhase();
        });

    int32 MaxPhase = 3;

    for (int32 Phase = 0; Phase <= MaxPhase; Phase++)
    {
        for (auto& Sys : Subsystems)
            if (Sys->GetSavePhase() == Phase)
                Sys->PreLoad();

        for (auto& Sys : Subsystems)
            if (Sys->GetSavePhase() == Phase)
                Sys->LoadData(*LoadedFile);

        for (auto& Sys : Subsystems)
            if (Sys->GetSavePhase() == Phase)
                Sys->PostLoad();
    }
}

void USaveSystem::LoadGameFromSlots(FString SaveName)
{

    USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(TEXT("SAVEDMAN"), 0);
    if (LoadedGame)
    {
        LoadedFile = Cast<USavedGame>(LoadedGame);
        if (LoadedFile)
        {
            LoadedSave = true;
        }
        else
        {
            LoadedSave = false;
        }
    }

}



