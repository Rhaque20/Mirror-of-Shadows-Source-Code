#include "RPGCharacterBase.h"
#include "GAS/CustomAbilitySystemComponent.h"

#include "GAS/BaseAttributeSet.h"
#include "DataAsset/Skill.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/StatusEffectComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/StaggerComponent.h"
#include "GAS/UDGameplayTags.h"


UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const 
{
    return AbilitySystem;
}

bool ARPGCharacterBase::IsDead() const
{
	return AbilitySystem->HasMatchingGameplayTag(TAG_Effect_State_Dead);
}

bool ARPGCharacterBase::IsImmobile() const
{
	return AbilitySystem->HasAnyMatchingGameplayTags(ImmobilizedTags);
}

void ARPGCharacterBase::StartDodge()
{
	StopDodgeTimeLine();
}

void ARPGCharacterBase::ActivateSkill()
{

}

bool ARPGCharacterBase::IsOnGround() const
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		return true;
	}

	if (!OnGround)
	{
		return false;
	}

	float CapsuleHalfHeightOffset = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector ActorLocationRef = GetActorLocation();
	FVector StartPoint = FVector(ActorLocationRef.X,ActorLocationRef.Y,ActorLocationRef.Z - CapsuleHalfHeightOffset);
	FVector EndPoint = StartPoint - FVector(0, 0, GroundCheckDistance);

	
	FHitResult HitResult;
	FCollisionQueryParams QueryParameters;

	QueryParameters.AddIgnoredActor(this);
	
	//if (HitGround)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Ground distance is %f for %s"), HitResult.Distance,*GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Line trace didn't hit ground for %s"),*GetName());
	//}

	// Trace Channel 5 is ground
	return GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECollisionChannel::ECC_GameTraceChannel5, QueryParameters);
}

void ARPGCharacterBase::AutoTarget()
{
}

void ARPGCharacterBase::AutoTarget_Set(AActor* Target)
{
}

void ARPGCharacterBase::AttackForceUpdate(float val)
{
	float HorizontalCurveVal = 0.0f, VerticalForce = GetCharacterMovement()->Velocity.Z;
	FVector AttackDirection;
	if (ActiveSkill)
	{
		if(ActiveSkill->GetLaunchCurve())
			HorizontalCurveVal = ActiveSkill->GetLaunchCurve()->GetFloatValue(val);

		/*if (ActiveSkill->ReturnHasVerticalForce())
		{
			VerticalForce = ActiveSkill->GetVerticalLaunchCurve()->GetFloatValue(val) * ActiveSkill->GetVerticalLaunchForce();
			UE_LOG(LogTemp, Display, TEXT("Use Vertical force of %f "), VerticalForce);
		}*/
		AttackDirection = FVector(ActiveSkill->GetLaunchForce() * GetActorForwardVector() * HorizontalCurveVal);
		SetVelocity(FVector(AttackDirection.X,AttackDirection.Y,VerticalForce));

	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Active skill is null in force update"));
	}
}

void ARPGCharacterBase::TriggerAirTime(float AirTime)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AirTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(AirTimerHandle);
		UE_LOG(LogTemp, Display, TEXT("Cleared air timer"));
	}

	GetWorld()->GetTimerManager().SetTimer(
		AirTimerHandle, // handle to cancel timer at a later time
		this, // the owning object
		&ARPGCharacterBase::EndAirTime, // function to call on elapsed
		AirTime, // float delay until elapsed
		false); // looping?
}

void ARPGCharacterBase::TriggerAirTimeManual(bool bSuspendAir)
{

}

void ARPGCharacterBase::EndAirTime()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AirTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(AirTimerHandle);
	}
}

void ARPGCharacterBase::StartAttackPropel(USkill* SkillData)
{
	CanMove = false;
	ActiveSkill = SkillData;

	UE_LOG(LogTemp, Display, TEXT("Received skilldata %s"),*(ActiveSkill->GetSkillName()));

	StopLaunchTimeLine();
	AttackForceTimeline.SetTimelineLength(ActiveSkill->GetAnimation(0)->GetPlayLength());
	AttackForceTimeline.PlayFromStart();
}

void ARPGCharacterBase::SetVelocity(FVector VelocityChange)
{
	GetMovementComponent()->Velocity = VelocityChange;
}

void ARPGCharacterBase::SetSkillModifier(float modifier)
{
	if (AttributeSet)
	{
		AttributeSet->SetSkillModifier(modifier);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%s attribute set is null"), *GetName());
	}
}

void ARPGCharacterBase::StopLaunchTimeLine()
{
	if (AttackForceTimeline.IsPlaying())
		AttackForceTimeline.Stop();
}

void ARPGCharacterBase::StopDodgeTimeLine()
{
	if (DodgeTimeline.IsPlaying())
		DodgeTimeline.Stop();
}

void ARPGCharacterBase::HardPlungeState(bool bHardPlungeToggle, float DiveVelocity)
{
	HardPlungeVelocity = DiveVelocity;
	bInHardPlunge = bHardPlungeToggle;

	if (bInHardPlunge)
	{
		UE_LOG(LogTemp, Display, TEXT("Canceling momentum"));
		SetVelocity(FVector::Zero());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Ending hard plunge state"));
	}
		
}

void ARPGCharacterBase::DodgeFunction(float val)
{
	FVector FacingDir = GetActorForwardVector() * 1500.f * val;
	float ZVelocity = GetCharacterMovement()->Velocity.Z;
	UE_LOG(LogTemp, Display, TEXT("Setting velocity in dodge function of ARPGCharacterBase"));
	SetVelocity(FVector(FacingDir.X, FacingDir.Y, ZVelocity));

}

void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Calling RPGCharacterBase BeginPlay"));

	InterpFunction.BindUFunction(this, "AttackForceUpdate");

	DodgeInterpFunction.BindUFunction(this, "DodgeFunction");

	DodgeTimeline.AddInterpFloat(DodgeCurve, DodgeInterpFunction);
	
	AttackForceTimeline.AddInterpFloat(DefaultTimeCurve, InterpFunction);
	DodgeTimeline.SetTimelineLength(0.6f);

	AttachWeaponsToSockets();
	InitializeAttributes();

	OriginalMoveSpeed = GetCharacterMovement()->GetMaxSpeed();
}

void ARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttackForceTimeline.TickTimeline(DeltaTime);
	DodgeTimeline.TickTimeline(DeltaTime);
}

ARPGCharacterBase::ARPGCharacterBase() 
{
    HitBoxLocation = CreateDefaultSubobject<USceneComponent>("Hitbox Spot Visual");
    HitBoxLocation->SetupAttachment(RootComponent);
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("BaseAttributeComponent");

	WeaponProp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon 1"));
	WeaponProp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon 2"));

	StatusEffectComponent = CreateDefaultSubobject<UStatusEffectComponent>("Status Effect Component");
}

void ARPGCharacterBase::AttachWeaponsToSockets() 
{
    if(IsValid(WeaponProp1))
	{
		WeaponProp1->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true),Weapon1SocketName);
	}

	if(IsValid(WeaponProp2))
	{
		WeaponProp2->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true),Weapon2SocketName);
	}
}

void ARPGCharacterBase::InitializeAttributes()
{
	if (!IsValid(AbilitySystem))
	{
		return;
	}
	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing an attribute ability! Set it in the blueprint!"), *GetName());
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystem->MakeOutgoingSpec(DefaultAttributes, Level, EffectContext);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystem->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystem);

		UE_LOG(LogTemp, Error, TEXT("Base stats have been applied"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Gameplay Effect handle is invalid"));
	}
}

void ARPGCharacterBase::PoiseBreak()
{
	if (!AbilitySystem->HasMatchingGameplayTag(TAG_Effect_State_PoiseBroken))
	{
		UE_LOG(LogTemp, Display, TEXT("Granting stagger effect"));
		FGameplayEffectSpecHandle Spec = AbilitySystem->MakeOutgoingSpec(PoiseBrokenEffect, 1, AbilitySystem->MakeEffectContext());
		AbilitySystem->ApplyGameplayEffectSpecToSelf(*(Spec.Data).Get());
	}

	UE_LOG(LogTemp, Display, TEXT("POISE BREAK in rpg character"));

	//FGameplayEventData StaggerEventData = FGameplayEventData();

	//StaggerEventData.OptionalObject2 = LightStaggerMontage;

	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag("Event.Combat.Hit"), StaggerEventData);
}

void ARPGCharacterBase::StunBreak()
{
	if (!AbilitySystem->HasMatchingGameplayTag(TAG_Effect_State_Stagger_Stunned))
	{
		UE_LOG(LogTemp, Display, TEXT("Granting stance break effect"));
		FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(StunEffect, 1, AbilitySystem->MakeEffectContext());
		FGameplayEffectSpec Spec = *(SpecHandle.Data).Get();
		Spec.SetSetByCallerMagnitude(TAG_Effect_State_Stagger_Stunned,StunTime);
		AbilitySystem->ApplyGameplayEffectSpecToSelf(Spec);
	}

	UE_LOG(LogTemp, Display, TEXT("Stance BREAK in rpg character"));
}
