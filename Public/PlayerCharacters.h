// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "AbilitySystemInterface.h"
#include "RPGCharacterBase.h"
#include "EnumLib.h"
#include "GAS/CustomAbilitySystemComponent.h"

#include "PlayerCharacters.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillHasActivated, APlayerCharacters*, ActivatingActor, class UPlayerSkill*, ActivatedSkill);

class UStaggerComponent;
class APlayerPartyController;
UCLASS()
class MIRROROFSHADOWS_API APlayerCharacters : public ARPGCharacterBase,public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacters();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetDamage() const;
	UFUNCTION(BlueprintCallable)
	void SetRotationByInput();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const struct FInputActionValue& InputValue);
	void Look(const struct FInputActionValue& InputValue);
	void LookUpRate(const struct FInputActionValue& InputValue);
	void Jump();

	virtual void TriggerAirTime(float AirTime = 2.0f) override;
	virtual void TriggerAirTimeManual(bool bSuspendAir) override;
	virtual void EndAirTime() override;

	virtual void DodgeFunction(float val) override;

public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OwnedTags) const override
	{
		OwnedTags = GameplayTagContainer;
		return;
	}

	bool GetIsPerformingAerialAction()
	{
		return bIsPerformingAerialAction;
	}

public:
	UPROPERTY(BlueprintAssignable, Category = "SkillChainActivate")
	FSkillHasActivated SkillActivated;

public:	
	UFUNCTION(BlueprintCallable)
	void SetPlayerActive(bool IsActive);

	void SwapIn();
	void SwapOut();

	virtual void StartAttackPropel(class USkill* SkillData) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void NormalAttackAbility();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void CounterEdgeAbility();// To be used for when performing a switch counter edge

	UFUNCTION(BlueprintCallable)
	void SetUpLockOn(bool LockOnToggle, AActor* Target);

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void PlayerRecover();

	virtual void AutoTarget() override;
	virtual void AutoTarget_Set(AActor* Target) override;

	UFUNCTION(BlueprintCallable)
	FRotator GetRotationByInput() const;

	UFUNCTION(BlueprintCallable)
	bool NormalAttack();

	UFUNCTION(BlueprintCallable)
	bool ChargeAttack();

	UFUNCTION(BlueprintCallable)
	bool SkillAttack(int index);

	UFUNCTION(BlueprintCallable)
	bool SkillAttackBySkill(class UPlayerSkill *SkillRef);

	UFUNCTION(BlueprintCallable)
	bool SummonAttack(AActor* Target, bool bUseGroundSkill);

	UFUNCTION(BlueprintCallable)
	void ResetMovementCache();

	UFUNCTION(BlueprintCallable)
	void GainSkillEnergy(float BaseEnergy, bool bUseSPGain = true);

	UFUNCTION(BlueprintCallable)
	void ExpendSkillEnergy(UPlayerSkill* SkillRef, float EnergyRatio = 1.0f);

	void GainSkillEnergy(float BaseEnergy, FGameplayTag SkillElement, float MatchModifier, float MismatchModifier, bool bUseSPGain = true);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateSkillDisplay();

	void GiveControllerRef(APlayerPartyController* ControllerRef);

	UFUNCTION(BlueprintCallable)
	bool GetIsSummoned() const
	{
		return bIsSummoned;
	}

	UFUNCTION(BlueprintCallable)
	bool IsInSkill() const;

	void SetIsSummoned(bool value);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly ,Category = "Camera Component")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Camera Component")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Combat", meta = (ToolTip = "Used for pulling flinching enemies towards it"))
	class USceneComponent* GravityOrb;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* LookUpRateAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* NormalAttackAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat Component")
	class UPlayerCore* CharacterCore;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UPlayerAttributeSet* PlayerAttributes;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay Tag")
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(BlueprintReadWrite, Category = "Controller")
	APlayerPartyController *PartyController;

	// UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "StaggerComponent")
	// UStaggerComponent* StaggerComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment")
	class UEquipmentComponent* EquipmentComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="Lock On System")
	bool HasLockOn = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="Lock On System")
	AActor* LockOnTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lock On System")
	AActor* CounterTarget;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Defense Action")
	UAnimMontage* ForwardRollMontage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Defense Action")
	UAnimMontage* BackRollMontage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Defense Action")
	UAnimMontage* ParryAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Defense Action")
	bool CanBlock = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsInSkill = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Defense Action")
	bool CanEvade = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Defense Action")
	bool bIsSummoned = false;

	UPROPERTY(EditAnywhere)
	float RotationRate = 10.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<class UGameplayAbility> NormalAttackClass;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<class UGameplayAbility> AirAttackClass;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<class UGameplayAbility> ChargeAttackClass;

	UPROPERTY(BlueprintReadOnly)
	FVector2D LastMoveInput;

	UPROPERTY(BlueprintReadWrite)
	bool CanUseLastMoveInput = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (ToolTip = "Determines if the player performs an action in the air ie) attacking, dodging"));
	bool bIsPerformingAerialAction = false;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Data")
	TArray<class UPlayerSkill*> GroundSkills;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Data")
	TArray<class UPlayerSkill*> AirSkills;*/

};