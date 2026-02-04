// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/InvisibleBarrier.h"

#define PlayerPawn ECollisionChannel::ECC_GameTraceChannel2;
#define PlayerTrace ECollisionChannel::ECC_GameTraceChannel3;
#define Enemy ECollisionChannel::ECC_GameTraceChannel4;

// Sets default values
AInvisibleBarrier::AInvisibleBarrier()
{
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));

    TriggerBox->SetBoundsScale(2.5f);
    TriggerBox->SetCollisionProfileName(TEXT("InvisibleWall"));
    TriggerBox->SetGenerateOverlapEvents(true);
    BarrierOff();
    PrimaryActorTick.bCanEverTick = false;
}

void AInvisibleBarrier::BarrierOn()
{
    TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Block);
    TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Block);
    TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECR_Block);

}

void AInvisibleBarrier::BarrierOff()
{
    TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Overlap);
    TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Overlap);
    TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECR_Block);
}

// Called when the game starts or when spawned
void AInvisibleBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

