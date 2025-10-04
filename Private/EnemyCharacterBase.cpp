// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GAS/EnemyAttributeSet.h"
#include "Components/StaggerComponent.h"
#include "Components/SkillCooldownManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "DataAsset/EnemySkill.h"

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

	bool inRange = false, inHeight = false, inAngle = false;

	for (UEnemySkill* skillRef : Moveset)
	{
		if (SkillCooldownManager->IsSkillOnCooldown(skillRef) || skillRef->IsSkillReaction())
			continue;

		inRange = UKismetMathLibrary::InRange_FloatFloat(Distance, skillRef->GetMinRangeReq(), skillRef->GetMaxRangeReq());
		heightReq = skillRef->GetHeightReq();
		inHeight = UKismetMathLibrary::InRange_FloatFloat(Height, -heightReq, heightReq);
		skillAngle = skillRef->GetAngleReq();
		inAngle = skillAngle > 0.0f ? Angle <= skillAngle : true;

		if (inRange && inHeight && inAngle)
		{
			ChosenSkill = skillRef;
		}
	}

	ActiveSkill = ChosenSkill;

	return ChosenSkill;
}

void AEnemyCharacterBase::SetSkillCooldown(UEnemySkill* SkillToCD)
{
	SkillCooldownManager->SetSkillCooldown(SkillToCD);
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

