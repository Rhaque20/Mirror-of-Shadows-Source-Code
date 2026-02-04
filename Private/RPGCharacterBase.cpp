#include "RPGCharacterBase.h"
#include "GAS/CustomAbilitySystemComponent.h"

#include "GAS/BaseAttributeSet.h"
#include "DataAsset/Skill.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/StatusEffectComponent.h"
#include "GameplayAbilities/Public/GameplayEffectTypes.h"
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

void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Calling RPGCharacterBase BeginPlay"));

	AttachWeaponsToSockets();
	InitializeAttributes();

	OriginalMoveSpeed = GetCharacterMovement()->GetMaxSpeed();
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetMoveSpeedAttribute()).AddUObject(this,&ARPGCharacterBase::OnMoveSpeedChange);
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

void ARPGCharacterBase::OnMoveSpeedChange(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = OriginalMoveSpeed * Data.NewValue;
}

void ARPGCharacterBase::InitializeAttributes(int level)
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

	if (level > Level)
	{
		Level = level;
	}

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
