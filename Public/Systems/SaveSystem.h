// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SavedGame.h"
#include "Interfaces/ISavableInterface.h"
#include "SaveSystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MIRROROFSHADOWS_API USaveSystem : public UObject
{
    GENERATED_BODY()

public:

    UPROPERTY()
    bool LoadedSave;

private:

    UPROPERTY()
    TObjectPtr<USavedGame> SaveFile;
    UPROPERTY()
    TObjectPtr<USavedGame> LoadedFile;
    TArray<TScriptInterface<ISavableInterface>> Subsystems;

public:

    virtual void PostInitProperties() override;

    UFUNCTION(BlueprintCallable)
    void RegisterSubsystem(TScriptInterface<ISavableInterface> Subsystem);

    UFUNCTION(BlueprintCallable)
    bool FindSubsystem(TScriptInterface<ISavableInterface> Subsystem);

     UFUNCTION(BlueprintCallable)
     void SaveGame();

     UFUNCTION(BlueprintCallable)
     void LoadGame();

     UFUNCTION(BlueprintCallable)
     void LoadGameFromSlots(FString SaveName);
    
    UFUNCTION(BlueprintCallable)
    USavedGame* RetrieveLoadedFile() const
    {
        return LoadedFile;
    }
    

};
