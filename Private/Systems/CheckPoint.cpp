// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CheckPoint.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
    RootComponent = TriggerSphere;

    TriggerSphere->InitSphereRadius(200.f);
    TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
    TriggerSphere->SetGenerateOverlapEvents(true);
}
