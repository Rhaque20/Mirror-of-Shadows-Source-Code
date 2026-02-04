// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "Enumerator/InteractionTypeEnum.h"

#include "InteractableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract,AInteractableActor*, InteractedActor);
UCLASS()
class MIRROROFSHADOWS_API AInteractableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UnlockInteractable();
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteract OnInteractEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnActivation();
	
protected:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, meta = (ToolTip="Used for one time activatable interactables"))
	bool bHasActivated = false;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, meta = (ToolTip="Used for one time activatable interactables"))
	EInteractiveType InteractiveType = EInteractiveType::Normal;

};
