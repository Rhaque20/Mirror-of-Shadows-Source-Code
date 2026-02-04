// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Systems/InvisibleBarrier.h"
#include "Components/BoxComponent.h"
#include "InvisibleBarrierSystem.generated.h"

USTRUCT(BlueprintType)
struct FBarrierData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
    FTransform Transform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector BoxExtent = FVector(128,128,128);
};

UCLASS()
class MIRROROFSHADOWS_API AInvisibleBarrierSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInvisibleBarrierSystem();

    virtual void OnConstruction(const FTransform& Transform) override;

    UFUNCTION(BlueprintCallable)
    void BarriersOn(AActor* PlayerRef);

    UFUNCTION(BlueprintCallable)
    void BarriersOff(AEnemySpawnerSystem* DefeatedSpawner);

    UPROPERTY(BlueprintReadWrite, Category = "Barriers")
    TArray<UBoxComponent*> ArenaWalls;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barriers", meta = (EditFixedSize = false))
    TArray<FBarrierData> BarrierBounds;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
