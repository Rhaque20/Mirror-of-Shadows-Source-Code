// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayModeBase.generated.h"

/**
 * 
 */
 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerControllerSpawn, APlayerController*,PlayerController);
UCLASS()
class MIRROROFSHADOWS_API AGameplayModeBase : public AGameModeBase
{
	GENERATED_BODY()

	public:
		void StartGame();
		virtual void BeginPlay() override;
		virtual void PostLogin(APlayerController* NewPlayer) override;
		
	public:
		FOnPlayerControllerSpawn OnPlayerControllerSpawn;
	
};
