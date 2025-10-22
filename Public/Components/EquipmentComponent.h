// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Structs/ArmorDataStruct.h"

#include "EquipmentComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable )
class MIRROROFSHADOWS_API UEquipmentComponent : public UActorComponent,public IGameplayTagAssetInterface
{
    GENERATED_BODY()
    protected:
        UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	    TArray<class UArmor*> EquippedGear;

        UPROPERTY(BlueprintReadWrite, Category = "Gameplay Tag")
	    FGameplayTagContainer GameplayTagContainer;

        UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Equipment")
        TMap<FGameplayTag, FArmorData> EquippedArmor;

        UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category = "EquipmentEffect")
        TSubclassOf<class UGameplayEffect> HelmetEffect;

        UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EquipmentEffect")
        TSubclassOf<class UGameplayEffect> ChestPlateEffect;

        UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EquipmentEffect")
        TSubclassOf<class UGameplayEffect> BracerEffect;

        UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EquipmentEffect")
        TSubclassOf<class UGameplayEffect> NecklaceEffect;

        UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EquipmentEffect")
        TSubclassOf<class UGameplayEffect> RingEffect;

        UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "EquipmentEffect")
        TSubclassOf<class UGameplayEffect> RingElementalEffect;

        class UAbilitySystemComponent *OwningASC;

    public:
        void BeginPlay() override;
        UFUNCTION(BlueprintCallable)
        void CheckSetBonus();

        UFUNCTION()
        void TurnArmorIntoStats(FArmorData ArmorEquipped);

        UFUNCTION(BlueprintCallable)
        bool EquipArmorInSlot(FArmorData ArmorToWear,FArmorData& OccupiedArmor);

        UFUNCTION(BlueprintCallable)
        FArmorData UnEquipArmor(FGameplayTag SlotToUnequip);

        UFUNCTION(BlueprintCallable)
        FArmorData GetArmorDataFromSlot(FGameplayTag EquipmentSlot) const;

        virtual void GetOwnedGameplayTags(FGameplayTagContainer& OwnedTags) const override
        {
            OwnedTags = GameplayTagContainer; 
            return;
        }
};
