// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayModeBase.h"
#include "PlayerPartyController.h"
#include "DataAsset/PlayerData.h"
#include "PlayerCharacters.h"

#include "Kismet/GameplayStatics.h"


void AGameplayModeBase::BeginPlay()
{
    Super::BeginPlay();
    StartGame();
}

void AGameplayModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    OnPlayerControllerSpawn.Broadcast(NewPlayer);
    OnPlayerControllerSpawn.RemoveAll(this);
}

void AGameplayModeBase::StartGame()
{
}
