// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CheckPoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectDetected, ACheckPoint*, activePoint);

UCLASS()
class ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision");
    USphereComponent* TriggerSphere;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Player Position")
    FOnObjectDetected UpdateP1Position;
};
