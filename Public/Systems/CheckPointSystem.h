// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint.h"
#include "GameFramework/Actor.h"
#include "CheckPointSystem.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckpointUpdate, FVector, CheckpointLocation);
UCLASS()
class MIRROROFSHADOWS_API ACheckPointSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPointSystem();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoint Presets")
    TArray<ACheckPoint*> CheckPointList;

    UPROPERTY(VisibleAnywhere, Category = "Player Position")
    uint32 P1ChkPnt;

	UPROPERTY(BlueprintAssignable, Category = "Player Position")
	FOnCheckpointUpdate OnCheckpointUpdate;

    UFUNCTION()
    void UpdatePlayerPosition(ACheckPoint* activePoint);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
