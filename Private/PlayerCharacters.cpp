// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacters.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "GAS/PlayerAttributeSet.h"
#include "PlayerCore.h"
#include "Components/EquipmentComponent.h"
#include "Components/StaggerComponent.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "DataAsset/PlayerSkill.h"
#include "PlayerPartyController.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/NavMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/TimelineComponent.h"
#include "GAS/CharacterGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/UDGameplayTags.h"

#define BufferTag "Event.Melee.Continue"

// Sets default values
APlayerCharacters::APlayerCharacters()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	AbilitySystem = CreateDefaultSubobject<UCustomAbilitySystemComponent>("AbilitySystem");
	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>("PlayerAttributeSet");
	PlayerAttributes = Cast<UPlayerAttributeSet>(AttributeSet);

	CharacterCore = CreateDefaultSubobject<UPlayerCore>(TEXT("PlayerCore"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment Component"));
	// StaggerComponent = CreateDefaultSubobject<UStaggerComponent>(TEXT("Stagger Component"));

	GravityOrb = CreateDefaultSubobject<USceneComponent>(TEXT("Gravity Orb"));
	GravityOrb->SetupAttachment(RootComponent);

	// bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

float APlayerCharacters::GetHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHP();
	}

	return 0.0f;
}

float APlayerCharacters::GetCurrentHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetCurrentHP();
	}

	return 0.0f;
}

void APlayerCharacters::SetUpLockOn(bool LockOnToggle, AActor* Target) 
{
	HasLockOn = LockOnToggle;
	LockOnTarget = Target;
}

void APlayerCharacters::AutoTarget()
{
	AActor* TargetRef = nullptr;
	if (HasLockOn)
	{
		TargetRef = LockOnTarget;
	}
	else if (IsValid(CounterTarget))
	{
		TargetRef = CounterTarget;
	}

	if(IsValid(TargetRef))
		AutoTarget_Set(TargetRef);
	else
	{
		SetRotationByInput();
	}

	
}

void APlayerCharacters::AutoTarget_Set(AActor* Target)
{
	if (IsValid(Target))
	{
		UE_LOG(LogTemp, Display, TEXT("Lock on is valid as target is %s"),*(Target->GetName()));
		FRotator PlayerRot = GetActorRotation();
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
		SetActorRotation(UKismetMathLibrary::MakeRotator(PlayerRot.Roll, PlayerRot.Pitch, LookAtRot.Yaw));
	}
}

FRotator APlayerCharacters::GetRotationByInput() const
{
	if (!CanUseLastMoveInput || !IsValid(Controller))
	{
		return GetActorRotation();
	}
	
	FRotator Rotation = Controller->GetControlRotation();
	FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(Rotation);
	FVector RightDirection = UKismetMathLibrary::GetRightVector(Rotation);

	FRotator FinalRotation = UKismetMathLibrary::FindLookAtRotation(ForwardDirection * LastMoveInput.Y * -1.0f,RightDirection* LastMoveInput.X);

	return FinalRotation;
}

bool APlayerCharacters::NormalAttack()
{
	bool SuccessfulAttack = false;

	if (IsValid(AbilitySystem))
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			SuccessfulAttack = AbilitySystem->TryActivateAbilityByClass(NormalAttackClass);
			//if (SuccessfulAttack)
			//	ActiveSkill = (USkill*)CharacterCore->ReturnActiveSkill(EAttackCategory::NormalAttack);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Air Attack"));
			SuccessfulAttack = AbilitySystem->TryActivateAbilityByClass(AirAttackClass);
			if(SuccessfulAttack)
				UE_LOG(LogTemp, Display, TEXT("Successful Air attack"));
		}

		if (!SuccessfulAttack)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag(BufferTag), FGameplayEventData());
		}
		
	}

	return SuccessfulAttack;
}

bool APlayerCharacters::ChargeAttack()
{
	bool SuccessfulAttack = false;

	if (IsValid(AbilitySystem))
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			SuccessfulAttack = AbilitySystem->TryActivateAbilityByClass(ChargeAttackClass);
			if (SuccessfulAttack)
				ActiveSkill = CharacterCore->GetHeavyAttack();
		}
	}
	return SuccessfulAttack;
}

bool APlayerCharacters::SkillAttack(int index)
{
	TArray<UPlayerSkill*> SkillList = CharacterCore->GetSkillList(OnGround);
	bool SuccessfulAttack = false;
	
	if (index >= 0 && index < SkillList.Num())
	{
		SkillAttackBySkill(SkillList[index]);
	}

	return SuccessfulAttack;
}

bool APlayerCharacters::SkillAttackBySkill(UPlayerSkill* SkillRef)
{
	if (!IsValid(SkillRef)) 
	{
		UE_LOG(LogTemp, Display, TEXT("Invalid skillref"));
		return false;
	}
		

	UE_LOG(LogTemp, Display, TEXT("SkillRef is valid"));

	bool SuccessfulAttack = false;

	if (SkillRef->IsReadyToUse())
	{
		TSubclassOf<UCharacterGameplayAbility> Classptr = SkillRef->ReturnAbilityClass();

		if (Classptr)
		{
			AbilitySystem->TryActivateAbilityByClass(Classptr);
		}
		else
		{
			FGameplayEventData Payload;
			Payload.OptionalObject = SkillRef;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_Ability_Input_Skill, Payload);
		}
	}

	return SuccessfulAttack;
}

bool APlayerCharacters::SummonAttack(AActor* Target, bool SummonerOnGround)
{
	bool SuccessfulAttack = false;
	ARPGCharacterBase* targetChar;

	bool useSkillOnGround;

	if (bIsSummoned)
		return false;

	if (IsValid(PartyController))
	{
		int chainLevelReq = CharacterCore->GetChainLevelRequirement(SummonerOnGround);
		int currentChainLevel = PartyController->GetSkillChainLevel();
		if (chainLevelReq == currentChainLevel)
		{
			if (IsValid(AbilitySystem))
			{
				UPlayerSkill* SwitchSkill;
				if (Target != nullptr)
				{
					targetChar = Cast<ARPGCharacterBase>(Target);
					useSkillOnGround = targetChar->IsOnGround();
					SwitchSkill = CharacterCore->GetSwitchSkill(useSkillOnGround);
					
				}
				else
				{
					SwitchSkill = CharacterCore->GetSwitchSkill(SummonerOnGround);
					useSkillOnGround = SummonerOnGround;
				}
				
				SuccessfulAttack = SkillAttackBySkill(SwitchSkill);
				if (SuccessfulAttack)
				{
					AutoTarget_Set(Target);
					bIsSummoned = true;
					// Use this for dealing with clipping through ground
					GetCharacterMovement()->bRunPhysicsWithNoController = true;
					if (!useSkillOnGround)
					{
						GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
						TriggerAirTime(2.0f);
					}
				}					
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Did not meet the summon requirement. Current level is %d and required level %d"),currentChainLevel,chainLevelReq);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Party Controller is null"));
	}

	
	return SuccessfulAttack;
}

void APlayerCharacters::ResetMovementCache() 
{
	UE_LOG(LogTemp, Display, TEXT("Resetted movement cache"));
	CanUseLastMoveInput = false;
}

void APlayerCharacters::GainSkillEnergy(float BaseEnergy, bool bUseSPGain)
{
	if (bUseSPGain)
	{
		if (PlayerAttributes->GetSPGain() <= 0.0f)
		{
			UE_LOG(LogTemp, Display, TEXT("SPGain is not bonus, it's modifier"));
			BaseEnergy *= (1+PlayerAttributes->GetSPGain());
		}
		else
			BaseEnergy *= PlayerAttributes->GetSPGain();
	}

	CharacterCore->FillAllSkillEnergy(BaseEnergy);
	UpdateSkillDisplay();
}

void APlayerCharacters::ExpendSkillEnergy(UPlayerSkill* SkillRef, float EnergyRatio)
{
	SkillRef->ExpendSkillEnergy(EnergyRatio);
	UpdateSkillDisplay();
	SkillActivated.Broadcast(this, SkillRef);
}

void APlayerCharacters::GainSkillEnergy(float BaseEnergy, FGameplayTag SkillElement, float MatchModifier, float MismatchModifier, bool bUseSPGain)
{
	if (bUseSPGain)
	{
		if (PlayerAttributes->GetSPGain() <= 0.0f)
		{
			UE_LOG(LogTemp, Display, TEXT("SPGain is not bonus, it's modifier"));
			BaseEnergy *= (1 + PlayerAttributes->GetSPGain());
		}
		else
			BaseEnergy *= PlayerAttributes->GetSPGain();
	}

	CharacterCore->FillAllSkillEnergy(BaseEnergy,SkillElement,MatchModifier,MismatchModifier);
	UpdateSkillDisplay();
}

void APlayerCharacters::GiveControllerRef(APlayerPartyController* ControllerRef)
{
	PartyController = ControllerRef;
}

bool APlayerCharacters::IsInSkill() const
{
	return AbilitySystem->HasMatchingGameplayTag(TAG_Ability_Input_Skill);
}

void APlayerCharacters::SetIsSummoned(bool value)
{
	bIsSummoned = value;
}

float APlayerCharacters::GetDamage() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetDamage();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute set is null"));
	}

	return 0.0f;
}

void APlayerCharacters::SetRotationByInput()
{
	SetActorRotation(GetRotationByInput());
	ResetMovementCache();
}

// Called when the game starts or when spawned
void APlayerCharacters::BeginPlay()
{
	AbilitySystem->InitAbilityActorInfo(this, this);
	Super::BeginPlay();
}

void APlayerCharacters::SetPlayerActive(bool IsActive)
{
	SetActorHiddenInGame(!IsActive);
	SetActorEnableCollision(IsActive);
	if(!IsActive)
		GetCharacterMovement()->bRunPhysicsWithNoController = false;
}

void APlayerCharacters::SwapIn() 
{
	UE_LOG(LogTemp, Display, TEXT("%s got swapped in"),*(GetName()));
	CanMove = true;
}

void APlayerCharacters::SwapOut() 
{
	HasLockOn = false;
	LockOnTarget = NULL;
}

void APlayerCharacters::StartAttackPropel(USkill* SkillData)
{
	Super::StartAttackPropel(SkillData);
}

// Called to bind functionality to input
void APlayerCharacters::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem 
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping,0);
		}
	}

	if(UEnhancedInputComponent* Input = 
	CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction,ETriggerEvent::Triggered,this,&APlayerCharacters::Move);
		Input->BindAction(MoveAction,ETriggerEvent::Completed,this,&APlayerCharacters::ResetMovementCache);
		Input->BindAction(LookAction,ETriggerEvent::Triggered,this,&APlayerCharacters::Look);
		Input->BindAction(JumpAction,ETriggerEvent::Started,this,&APlayerCharacters::Jump);
		Input->BindAction(LookUpRateAction,ETriggerEvent::Triggered,this,&APlayerCharacters::LookUpRate);
	}

}

void APlayerCharacters::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller) && !IsImmobile())
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.f,Rotation.Yaw,0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
	}
	LastMoveInput = InputVector;
	CanUseLastMoveInput = true;
}

void APlayerCharacters::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();
	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void APlayerCharacters::LookUpRate(const struct FInputActionValue& InputValue) 
{
	FVector2D InputVector = InputValue.Get<FVector2D>();
	if (IsValid(Controller))
	{
		float RotationMod = RotationRate * GetWorld()->GetDeltaSeconds();
		AddControllerYawInput(InputVector.X * RotationMod);
		AddControllerPitchInput(InputVector.Y * RotationMod);
	}
}

void APlayerCharacters::Jump()
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_Ability_Input_Jump, FGameplayEventData());

	if (!IsImmobile())
	{
		ACharacter::Jump();
	}
}

void APlayerCharacters::TriggerAirTime(float AirTime)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AirTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(AirTimerHandle);
		UE_LOG(LogTemp, Display, TEXT("Cleared air timer"));
	}

	GetWorld()->GetTimerManager().SetTimer(
		AirTimerHandle, // handle to cancel timer at a later time
		this, // the owning object
		&APlayerCharacters::EndAirTime, // function to call on elapsed
		AirTime, // float delay until elapsed
		false);
	bIsPerformingAerialAction = true;

	if (!AirTimerHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Air Timer somehow didn't get set"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Air timer handle set."));
	}

}

void APlayerCharacters::TriggerAirTimeManual(bool bSuspendAir)
{
	bIsPerformingAerialAction = bSuspendAir;

	if (!bSuspendAir && GetWorld()->GetTimerManager().IsTimerActive(AirTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(AirTimerHandle);
	}
}

void APlayerCharacters::EndAirTime()
{
	Super::EndAirTime();
	bIsPerformingAerialAction = false;
	UE_LOG(LogTemp, Display, TEXT("Override EndAirTime called"));
}

void APlayerCharacters::DodgeFunction(float val)
{
	if (bIsPerformingAerialAction)
	{
		FVector FacingDir = GetActorForwardVector() * 1500.f * val;
		UE_LOG(LogTemp, Display, TEXT("Dodge function calling set velocity in APlayerCharacters"));
		SetVelocity(FVector(FacingDir.X, FacingDir.Y, 0.0f));
	}
	else
		Super::DodgeFunction(val);
}

