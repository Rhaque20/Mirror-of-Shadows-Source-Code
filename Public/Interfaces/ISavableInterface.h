// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISavableInterface.generated.h"

class USavedGame;

UINTERFACE(MinimalAPI, Blueprintable)
class USavableInterface : public UInterface
{
    GENERATED_BODY()
};

class MIRROROFSHADOWS_API ISavableInterface
{
    GENERATED_BODY()

public:
    virtual void InitializeData() {};
    virtual int32 GetSavePhase() const = 0;
    virtual void PreSave() {}
    virtual void SaveData(USavedGame& Root) = 0;
    virtual void PostSave() {}

    virtual void PreLoad() {}
    virtual void LoadData(const USavedGame& Root) = 0;
    virtual void PostLoad() {}
};
