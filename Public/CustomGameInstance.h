// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Systems/SaveSystem.h"
#include "CustomGameInstance.generated.h"  // Changed from MyGameInstance.generated.h

UCLASS()
class MIRROROFSHADOWS_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UMyGameInstance();

    void HandleWorldReady(UWorld* LoadedWorld);

    virtual void Init() override;

    UPROPERTY(BlueprintReadWrite, Category = "SaveSystem")
    USaveSystem* SaveSystem;

};

