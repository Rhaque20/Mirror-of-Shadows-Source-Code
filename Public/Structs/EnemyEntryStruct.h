#include "EnemyEntryStruct.generated.h"
#pragma once

USTRUCT(BlueprintType)
struct FEnemyEntryData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TSubclassOf<class AEnemyCharacterBase> EnemyClass;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere, meta = (MakeEditWidget))
	FTransform SpawnTransform;
};

USTRUCT(BlueprintType)
struct FEnemyGroupData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Editor Settings")
	bool bIsGroupVisible = true;
	
	UPROPERTY(EditAnywhere, Category = "Editor Settings")
	FColor GizmoColor = FColor::Green;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TArray<FEnemyEntryData> EnemyGroupArray;
};