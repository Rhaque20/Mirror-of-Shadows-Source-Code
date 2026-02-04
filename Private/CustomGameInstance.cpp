// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
    // ✅ Use CreateDefaultSubobject for objects created in constructor
    SaveSystem = CreateDefaultSubobject<USaveSystem>(TEXT("SaveSystem"));

}

void UMyGameInstance::HandleWorldReady(UWorld* LoadedWorld)
{
    if (SaveSystem->LoadedSave)
    {
        SaveSystem->LoadGame();
    }
}

void UMyGameInstance::Init()
{
     Super::Init();
    SaveSystem->LoadGameFromSlots("SAVEFILE");
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&UMyGameInstance::HandleWorldReady);
}



