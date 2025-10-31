// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/HitStopInterface.h"
#include "Components/TimelineComponent.h"
#include "GameplayTagContainer.h"

#include "RPGCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, ARPGCharacterBase*, charBase);

class UStaggerComponent;
class UStatusEffectComponent;
class UCharacterGameplayAbility;
UCLASS()
class MIRROROFSHADOWS_API ARPGCharacterBase : public ACharacter,public IAbilitySystemInterface, public IHitStopInterface
{
    GENERATED_BODY()
    public:
        ARPGCharacterBase();
        virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

        UStaggerComponent* ReturnStaggerComponent() const
        {
            return StaggerComponent;
        }

        UStatusEffectComponent* GetStatusEffectComponent() const
        {
            return StatusEffectComponent;
        }

        int GetLevel() const
        {
            return Level;
        }

        UFUNCTION(BLueprintCallable)
        bool IsDead() const;

        UFUNCTION(BlueprintCallable)
        bool IsImmobile() const;

        UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
        void PerformDodge();

        UFUNCTION(BlueprintCallable)
        virtual void StartDodge();

        UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
        void CancelDodge();

        UFUNCTION(BlueprintCallable)
        virtual void ActivateSkill();

        UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
        void OnDeath();

        UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
        void DisplayDamageNumber(const struct FGameplayTagContainer& Container, float Damage, int DidHit);

        UFUNCTION(BlueprintCallable)
        bool IsOnGround() const;

        UFUNCTION(BlueprintCallable)
        virtual void AutoTarget();

        UFUNCTION(BlueprintCallable)
        virtual void AutoTarget_Set(AActor* Target);

        UFUNCTION()
        virtual void AttackForceUpdate(float val);

        UFUNCTION(BlueprintCallable)
        virtual void TriggerAirTime(float AirTime = 2.0f);

        UFUNCTION(BlueprintCallable)
        virtual void TriggerAirTimeManual(bool bSuspendAir);

        UFUNCTION(BlueprintCallable)
        virtual void EndAirTime();

        UFUNCTION(BlueprintCallable)
        virtual void StartAttackPropel(class USkill* SkillData);

        virtual void SetVelocity(FVector VelocityChange);

        void PoiseBreak();

        void StunBreak();

    protected:
        UFUNCTION(BlueprintCallable)
        void SetSkillModifier(float modifier);

        UFUNCTION(BlueprintCallable)
        void StopLaunchTimeLine();

        UFUNCTION(BlueprintCallable)
        void StopDodgeTimeLine();

        UFUNCTION(BlueprintCallable)
        void HardPlungeState(bool bHardPlungeToggle, float DiveVelocity);

        UFUNCTION()
        virtual void DodgeFunction(float val);

        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;

        void AttachWeaponsToSockets();

        void InitializeAttributes();

    public:
        UPROPERTY(BlueprintAssignable, BlueprintCallable,Category = "CharacterDeath")
        FOnDeath CharacterDied;

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        class UCustomAbilitySystemComponent* AbilitySystem;

        UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "ActorComponent")
	    UStaggerComponent* StaggerComponent;

        UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Mobility")
        bool CanMove = true;

        UPROPERTY(BlueprintReadWrite, Category = "Aerial Variables")
        bool OnGround = true;
        UPROPERTY(BlueprintReadWrite, Category = "Aerial Variables")
        float AerialVelocity = 0.f;
        UPROPERTY(BlueprintReadWrite, Category = "CurrentSkill")
        class USkill* ActiveSkill;

        UPROPERTY(BlueprintReadOnly, EditAnywhere,Category = "Hit System")
        USceneComponent* HitBoxLocation;

        UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
        TArray<TSubclassOf<UCharacterGameplayAbility>> Abilities;

        UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
        class UBaseAttributeSet* AttributeSet;

        UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        class UCurveFloat* DodgeCurve;

        UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
        class UCurveFloat* DefaultTimeCurve;

        FTimeline AttackForceTimeline;
        FTimeline DodgeTimeline;

        FOnTimelineFloat InterpFunction;
        FOnTimelineFloat DodgeInterpFunction;

        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ToolTip = "Responsible for delaying falling"))
        FTimerHandle AirTimerHandle;

        UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
        float GroundCheckDistance = 10.0f;

        UPROPERTY(BlueprintReadOnly)
        float OriginalMoveSpeed = 0.0f;

        UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
        float StunTime = 3.0f;

        UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "Weapon Props",meta = (ToolTip = "Static Mesh Reference For First Weapon"))
        UStaticMeshComponent* WeaponProp1;
        UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "Weapon Props",meta = (ToolTip = "Static Mesh Reference For Second Weapon"))
        UStaticMeshComponent* WeaponProp2;

        UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "WeaponProps", meta = (ToolTip = "Socket name for weapon 1"))
        FName Weapon1SocketName;
        UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "WeaponProps", meta = (ToolTip = "Socket name for weapon 2"))
        FName Weapon2SocketName;
        UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (ToolTip = "Used for toggling hard plunge or not"))
        bool bInHardPlunge = false;

        UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, meta =(ToolTip = "Set by skills to trigger"))
        float HardPlungeVelocity = 0.0f;

        UPROPERTY(EditDefaultsOnly)
        UAnimMontage* LightStaggerMontage;

        UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "Mechanical Effect", meta = (ToolTip = "Effect to grant upon getting poisebroken"))
        TSubclassOf<class UGameplayEffect> PoiseBrokenEffect;

        UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mechanical Effect", meta = (ToolTip = "Effect to grant upon getting stun at max"))
        TSubclassOf<class UGameplayEffect> StunEffect;

        UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mechanical Effect", meta = (ToolTip = "Stagger ability when poise broken or stance broken"))
        TSubclassOf<class UGameplayAbility> StaggerAbility;

        UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
        FGameplayTagContainer ImmobilizedTags;

        UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ActorComponent")
        UStatusEffectComponent* StatusEffectComponent;

        UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Level")
        int Level = 1;

        UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
        TSubclassOf<class UGameplayEffect> DefaultAttributes;
};




