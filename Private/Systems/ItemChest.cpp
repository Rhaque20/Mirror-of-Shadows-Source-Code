// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/ItemChest.h"

#include "Systems/EnemySpawnerSystem.h"
#include "NiagaraComponent.h"
#include "Structs/ItemSpec.h"

// Sets default values
AItemChest::AItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TestComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	TestComp->SetupAttachment(RootComponent);
	LockedVFX = CreateDefaultSubobject<UNiagaraComponent>("LockedVFX");
	LockedVFX->SetupAttachment(TestComp);
	LockedVFX->SetAutoActivate(false);
	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChestMesh");
	ChestMesh->SetupAttachment(TestComp);

}

void AItemChest::UnlockChest(AEnemySpawnerSystem* DefeatedSpawner)
{
	bIsLocked = false;
	LockedVFX->DeactivateImmediate();
}

void AItemChest::LockChest()
{
	bIsLocked = true;
	LockedVFX->Activate();
}

void AItemChest::ChestToggle(bool bSetVisible)
{
	SetActorHiddenInGame(!bSetVisible);
	SetActorEnableCollision(bSetVisible);
}

// Called when the game starts or when spawned
void AItemChest::BeginPlay()
{
	Super::BeginPlay();
}

