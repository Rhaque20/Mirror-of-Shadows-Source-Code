// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "InvisibleBarrier.generated.h"

UCLASS()
class MIRROROFSHADOWS_API AInvisibleBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInvisibleBarrier();

    UPROPERTY(EditAnywhere, Category = "Collision");
    UBoxComponent* TriggerBox;

    UFUNCTION(BlueprintCallable)
    void BarrierOn();

    UFUNCTION(BlueprintCallable)
    void BarrierOff();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
