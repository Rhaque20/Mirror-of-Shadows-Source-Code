// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/InvisibleBarrierSystem.h"
#include "Systems/EnemySpawnerSystem.h"

#define PlayerPawn ECollisionChannel::ECC_GameTraceChannel2;
#define PlayerTrace ECollisionChannel::ECC_GameTraceChannel3;
#define Enemy ECollisionChannel::ECC_GameTraceChannel4;

// Sets default values
AInvisibleBarrierSystem::AInvisibleBarrierSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    // Ensures every future compnent refers to this as the root
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent->SetMobility(EComponentMobility::Static);

    ArenaWalls.Reserve(4);
    BarrierBounds.Reserve(4);
}

/// @brief Called when an instance of this class is placed (in editor) or spawned.
/// @param Transform            The transform the actor was constructed at.
void AInvisibleBarrierSystem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // Clear previously generated walls
    for (UBoxComponent* Box : ArenaWalls)
    {
        if (Box)
        {
            Box->DestroyComponent();
        }
    }
    ArenaWalls.Empty();
    
    // Rebuild based on BarrierBounds
    for (int32 i = 0; i < BarrierBounds.Num(); ++i)
    {
        const FBarrierData& Data = BarrierBounds[i];
    
        // Generate a unique name to prevent collisions
        FName UniqueName = FName(*FString::Printf(TEXT("BarrierBox_%d_%d"), i, FMath::Rand()));
    
       
        // Create a new BoxComponent
        // UBoxComponent* NewBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), UniqueName);
    
        UActorComponent* Component = AddComponentByClass(UBoxComponent::StaticClass(),false,Data.Transform,false);
        UBoxComponent* NewBox = Cast<UBoxComponent>(Component);
    
        if (NewBox)
        {
            // Attach to root
            // NewBox->SetupAttachment(RootComponent);
    
            // Register with the engine
            NewBox->RegisterComponent();
    
            // Set transform and size
            // NewBox->SetRelativeTransform(Data.Transform);
            NewBox->SetBoxExtent(Data.BoxExtent);
    
            // Visual and collision settings
            NewBox->SetHiddenInGame(true);
            NewBox->SetVisibility(true);
            NewBox->SetMobility(EComponentMobility::Movable);
            NewBox->SetCanEverAffectNavigation(true);
    
            // Add to tracking array
            ArenaWalls.Add(NewBox);
        }
    }

}

void AInvisibleBarrierSystem::BarriersOn(AActor* PlayerRef)
{
    check(ArenaWalls.Num() > 0 && ArenaWalls.Max() > 0);
    for(int32 i = 0; i < ArenaWalls.Num(); ++i)
    {
        ArenaWalls[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
        ArenaWalls[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Block);
        ArenaWalls[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Block);
        ArenaWalls[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECR_Ignore);
        ArenaWalls[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Block);
    }
}

void AInvisibleBarrierSystem::BarriersOff(AEnemySpawnerSystem* DefeatedSpawner)
{
    check(ArenaWalls.Num() > 0 && ArenaWalls.Max() > 0);
    for (int32 i = 0; i < ArenaWalls.Num(); ++i)
    {
        ArenaWalls[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
        ArenaWalls[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Overlap);
        ArenaWalls[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Overlap);
        ArenaWalls[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECR_Ignore);
    }
}

// Called when the game starts or when spawned
void AInvisibleBarrierSystem::BeginPlay()
{
	Super::BeginPlay();
}

