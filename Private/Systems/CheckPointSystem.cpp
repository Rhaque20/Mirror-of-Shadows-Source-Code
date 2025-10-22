// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CheckPointSystem.h"

// Sets default values
ACheckPointSystem::ACheckPointSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/// @brief Assumes that the CheckPoint has identified the colliding object as the player 
/// @param activePoint The CheckPoint instance that got their OnComponentBeginOverlap event called
void ACheckPointSystem::UpdatePlayerPosition(ACheckPoint* activePoint)
{ 
    int32 newIdx = CheckPointList.Find(activePoint);
    if(newIdx >= 0)
    {
        UE_LOG(LogTemp, Display, TEXT("P1 CheckPoint Index: %d"), newIdx);
        P1ChkPnt = newIdx;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UpdatePlayerPosition(): Player Position failed to update due to the activePoint pointer not being found in CheckPointSystem's CheckPointList"));
        UE_LOG(LogTemp, Error, TEXT("CheckPointSystem's CheckPointList size: %d"), CheckPointList.Num());
    }
}

// Called when the game starts or when spawned
void ACheckPointSystem::BeginPlay()
{
	Super::BeginPlay();

    for(int32 i = 0; i < CheckPointList.Num(); ++i)
    {
        CheckPointList[i]->UpdateP1Position.AddDynamic(this, &ACheckPointSystem::UpdatePlayerPosition);
    }
}

// Called every frame
void ACheckPointSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

