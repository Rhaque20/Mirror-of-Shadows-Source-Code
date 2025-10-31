// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GAS/EnemyAttributeSet.h"
#include "GAS/UDGameplayTags.h"
#include "Components/StaggerComponent.h"
#include "Components/SkillCooldownManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "DataAsset/EnemySkill.h"
#include "GAS/AsyncTaskEffectStackChanged.h"
#include "GAS/CharacterGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystem = CreateDefaultSubobject<UCustomAbilitySystemComponent>("AbilitySystem");
	AttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>("AttributeSet");
	SkillCooldownManager = CreateDefaultSubobject<USkillCooldownManagerComponent>(TEXT("SkillCDManager"));
	// StaggerComponent = CreateDefaultSubobject<UStaggerComponent>(TEXT("Stagger Component"));

}

void AEnemyCharacterBase::AutoTarget()
{
	if (AttackTarget)
	{
		FRotator EnemyRot = GetActorRotation();
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
		SetActorRotation(UKismetMathLibrary::MakeRotator(EnemyRot.Roll, EnemyRot.Pitch, LookAtRot.Yaw));
	}
}

void AEnemyCharacterBase::ReceivePlayerTarget(AActor* PlayerRef)
{
	AttackTarget = PlayerRef;
	DetectedTarget = true;
	AEnemyAIController* ControllerRef = Cast<AEnemyAIController>(GetController());
	ControllerRef->SetPlayerPawn(Cast<APawn>(PlayerRef));
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	FVector velocity = MoveComp->Velocity;
	MoveComp->StopMovementImmediately();
	MoveComp->Velocity = velocity;
	OnDetectedPlayer.Broadcast(PlayerRef);
	
}

void AEnemyCharacterBase::CleanUp()
{
	OnRequestTicket.Unbind();
	OnReturnTicket.RemoveAll(this);
	OnStealTicket.Unbind();

}

void AEnemyCharacterBase::ReceiveTicket(FGameplayTag TicketToReceive)
{
	/*if (AbilitySystem->HasMatchingGameplayTag(TAG_Effect_State_AttackDelay))
	{
		return;
	}

	bool ReceivedATicket = OnRequestTicket.Execute(this, TAG_EnemyAI_Ticket_Melee);*/

	/*if (ReceivedATicket)
	{
		
	}*/

	CurrentTicket = TicketToReceive;
	GetWorld()->GetTimerManager().ClearTimer(DecisionTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(
		DecisionTimerHandle,
		this,
		&AEnemyCharacterBase::PreparingForAttack,
		DecisionTickInterval,
		true);
}

void AEnemyCharacterBase::ReturnTicket()
{
	CurrentTicket = FGameplayTag::EmptyTag;
	OnReturnTicket.Broadcast(this, TAG_EnemyAI_Ticket_Melee);
	GetWorld()->GetTimerManager().ClearTimer(DecisionTimerHandle);

	/*GetWorld()->GetTimerManager().SetTimer(
		DecisionTimerHandle,
		this,
		&AEnemyCharacterBase::RequestTicket,
		DecisionTickInterval,
		true);*/
}

void AEnemyCharacterBase::UseSkill(UEnemySkill* Skill)
{
	TSubclassOf<UCharacterGameplayAbility> Classptr = Skill->ReturnAbilityClass();

	if (Classptr)
	{
		AbilitySystem->TryActivateAbilityByClass(Classptr);
	}
	else
	{
		// THIS HAS TO GUARANTEE THAT IT WORKS
		FGameplayEventData Payload;
		Payload.OptionalObject = Skill;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_Ability_Input_NormalAttack, Payload);
	}
}

void AEnemyCharacterBase::PreparingForAttack()
{
	if (CurrentTicket == FGameplayTag::EmptyTag)
	{
		GetWorld()->GetTimerManager().ClearTimer(DecisionTimerHandle);

		/*GetWorld()->GetTimerManager().SetTimer(
			DecisionTimerHandle,
			this,
			&AEnemyCharacterBase::RequestTicket,
			DecisionTickInterval,
			true);*/

		return;
	}

	if (IsImmobile())
	{
		return;
	}

	UEnemySkill* SkillToUse = ReturnAvailableSkill();

	if (SkillToUse)
	{
		UseSkill(SkillToUse);
	}
}

void AEnemyCharacterBase::OnAttackDelayStatusChange(FGameplayTag DelayTag,FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount)
{
	if (DelayTag == TAG_Effect_State_AttackDelay && NewStackCount == 0)
	{
		/*if (!DecisionTimerHandle.IsValid())
		{
			UE_LOG(LogTemp, Display, TEXT("Number of instances of the tag is %d"), NewStackCount);
			GetWorld()->GetTimerManager().SetTimer(
				DecisionTimerHandle,
				this,
				&AEnemyCharacterBase::RequestTicket,
				DecisionTickInterval,
				true);
		}*/
		
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Responding to the wrong tag"));
	}
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (SkillCooldownManager)
	{
		SkillCooldownManager->SetUpSkillCooldowns(Moveset);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SKILL COOLDOWN MANAGER IS NULL!"));
	}

	// Create a task to handle listening to when gameplay effect stacks change, specifically attack delay
	//AttackDelayListener = UAsyncTaskEffectStackChanged::ListenForGameplayEffectStackChange_CPP(AbilitySystem, TAG_Effect_State_AttackDelay, true, FStackChangeFunction::CreateUObject(this, &AEnemyCharacterBase::OnAttackDelayStatusChange));

	/*GetWorld()->GetTimerManager().SetTimer(
		DecisionTimerHandle,
		this,
		&AEnemyCharacterBase::RequestTicket,
		DecisionTickInterval,
		true);*/
	
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacterBase::GetHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHP();
	}

	return 0.0f;
}

float AEnemyCharacterBase::GetCurrentHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetCurrentHP();
	}
	return 0.0f;
}

UEnemySkill* AEnemyCharacterBase::ReturnAvailableSkill()
{
	if (!AttackTarget)
		return nullptr;

	float Distance, Height, Angle;
	float skillAngle = 0.0f;
	float heightReq = 0.0f;
	GetConditionParameters(AttackTarget, Distance, Angle, Height);

	UEnemySkill* ChosenSkill = nullptr;

	bool inRange = false, inHeight = false, inAngle = false, inState = false;

	for (UEnemySkill* skillRef : Moveset)
	{
		if (SkillCooldownManager->IsSkillOnCooldown(skillRef) || skillRef->IsSkillReaction())
			continue;

		// Insert RequestTicket event invocation here (this enemy will carry the delegate)

		inRange = UKismetMathLibrary::InRange_FloatFloat(Distance, skillRef->GetMinRangeReq(), skillRef->GetMaxRangeReq());
		heightReq = skillRef->GetHeightReq();
		inHeight = UKismetMathLibrary::InRange_FloatFloat(Height, -heightReq, heightReq);
		skillAngle = skillRef->GetAngleReq();
		inAngle = skillAngle > 0.0f ? Angle <= skillAngle : true;
		FGameplayTag stateTag = skillRef->GetStateRequirement();
		if (stateTag == FGameplayTag::EmptyTag)
		{
			inState = true;
		}
		else
		{
			inState = AbilitySystem->HasMatchingGameplayTag(stateTag);
		}

		if (inRange && inHeight && inAngle && inState)
		{
			ChosenSkill = skillRef;
			break;
		}
	}

	ActiveSkill = ChosenSkill;

	return ChosenSkill;
}

void AEnemyCharacterBase::SetSkillCooldown(UEnemySkill* SkillToCD)
{
	SkillCooldownManager->SetSkillCooldown(SkillToCD);
}

void AEnemyCharacterBase::GiveFlyingPositions(TArray<FVector> FlyingPos)
{
	FlyingPositions = FlyingPos;
}

void AEnemyCharacterBase::Recover() 
{
	IsAttacking = false;
	CanMove = true;
}

void AEnemyCharacterBase::DetectionFill() 
{
	
}

void AEnemyCharacterBase::GetConditionParameters(AActor *TargetActor, float& Distance, float& Angle, float& Height) const
{
	FVector TargetLocation = TargetActor->GetActorLocation();
	// Distance between attacker (this) and the target (not necessarily the player)
	Distance = FVector::Dist(this->GetActorLocation(),TargetLocation);
	// Height difference between the attacker (this) and the target
	// Maybe should replace location with eye height location in the future.
	Height = TargetLocation.Z - this->GetActorLocation().Z;

	FVector AttackerVector = this->GetActorForwardVector();

	FVector Difference = TargetLocation - this->GetActorLocation();
	Difference.Z = 0;

	double DotProduct = UKismetMathLibrary::Dot_VectorVector(AttackerVector,Difference.GetSafeNormal());

	// Calculate the angle in radians
	float AngleInRadians = FMath::Acos(DotProduct);

	// Convert the angle to degrees between the actor
	Angle = FMath::RadiansToDegrees(AngleInRadians);
}

