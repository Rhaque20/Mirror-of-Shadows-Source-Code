// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/ItemChest.h"

#include "Structs/ItemSpec.h"

// Sets default values
AItemChest::AItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AItemChest::UnlockChest()
{
	bIsLocked = false;
}

void AItemChest::LockChest()
{
	bIsLocked = true;
}

// Called when the game starts or when spawned
void AItemChest::BeginPlay()
{
	Super::BeginPlay();
}

