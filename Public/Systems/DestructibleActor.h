// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "Enumerator/InteractionTypeEnum.h"
#include "Interfaces/DamageableInterface.h"

#include "DestructibleActor.generated.h"


UCLASS()
class MIRROROFSHADOWS_API ADestructableActor : public AActor, public IInteractableInterface, public IDamageableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void OnDestruction();
	
public:
	class AInteractableActor* GetOwningInteractor() const
	{
		return OwningInteractor;
	}
	
protected:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, meta = (ToolTip="Used for one time activatable interactables"))
	bool bHasActivated = false;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, meta = (ToolTip="Used for one time activatable interactables"))
	EInteractiveType InteractiveType = EInteractiveType::Normal;
	
	UPROPERTY(BlueprintReadOnly,VisibleInstanceOnly)
	FGuid DestructableID;
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float MaxHP = 100.0f;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	float CurrentHP = 0.0f;
	
	UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
	AInteractableActor* OwningInteractor = nullptr;

};
