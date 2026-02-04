// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DataAsset/ProjectileData.h"
#include "DataAsset/Skill.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GAS/UDGameplayTags.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TestComp = CreateDefaultSubobject<USceneComponent>("Scene Root");
	TestComp->SetupAttachment(RootComponent);
	
	ProjectileSphere = CreateDefaultSubobject<USphereComponent>(FName("Projectile Collision"));
	ProjectileSphere->SetCollisionObjectType(ECC_PhysicsBody);
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Projectile Mesh"));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileVFX = CreateDefaultSubobject<UNiagaraComponent>(FName("Projectile VFX"));
	
	ProjectileMoveComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	
	ProjectileSphere->SetupAttachment(TestComp);
	ProjectileMesh->SetupAttachment(TestComp);
	ProjectileVFX->SetupAttachment(TestComp);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::SetProjectileProperties(UProjectileData* ProjectileInfo,bool bStartLifeTime)
{
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("NO OWNER ON PROJECTILE %s"),*GetName());
		Destroy();
		return;
	}
	
	IAbilitySystemInterface* ImplementedOwner = Cast<IAbilitySystemInterface>(Owner);
	if (ImplementedOwner)
	{
		OwnerAbilitySystem = ImplementedOwner->GetAbilitySystemComponent();
	}
	else
	{
		Destroy();
		return;
	}
		
	
	ProjectileData = ProjectileInfo;
	
	float sphereRadius = 0.0f;
	bool bGrowOverTime = false;
	
	ProjectileData->GetSphereHitRadius(bGrowOverTime,sphereRadius,sphereRadius);
	
	ProjectileSphere->SetSphereRadius(sphereRadius);
	
	ProjectileVFX->SetAsset(ProjectileData->GetProjectileVFX());
	ProjectileSphere->OnComponentBeginOverlap.AddDynamic(this,&AProjectileBase::OnProjectileImpact);
	
	PlayerResponse = ProjectileData->GetPlayerResponse();
	EnemyResponse = ProjectileData->GetEnemyResponse();
	
	ProjectileSphere->SetCollisionResponseToChannel(ECC_Pawn, EnemyResponse != EFactionHitType::None ? ECR_Overlap : ECR_Ignore);
	ProjectileSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel12, PlayerResponse != EFactionHitType::None ? ECR_Overlap : ECR_Ignore);
	
	bool bUseGravity = ProjectileData->GetAffectedByGravity();
	ProjectileMoveComponent->ProjectileGravityScale = (bUseGravity ? 1.0f : 0.0f);
	
	if (bStartLifeTime)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ProjectileLifetimeHandle, // handle to cancel timer at a later time
			this, // the owning object
			&AProjectileBase::DestroyProjectile, // function to call on elapsed
			ProjectileData->GetProjectileLifeTime(), // float delay until elapsed
			false);// Make the timer loop
		
		if (ProjectileData->GetImpactPolicy() == EImpactPolicy::StickySurface)
		{
			StartGroundCheck();
		}
	}
	
	//UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpec,TAG_Event_Combat_Hit,0);
}

void AProjectileBase::FireProjectile(FVector Direction, FVector StartPos)
{
	if (ProjectileData != nullptr)
	{
		SetActorLocation(StartPos);
		float Speed = ProjectileData->GetProjectileSpeed();
		ProjectileMoveComponent->MaxSpeed = Speed;
		ProjectileMoveComponent->InitialSpeed = Speed;
		ProjectileMoveComponent->Velocity = (Speed * Direction);
		
		GetWorld()->GetTimerManager().SetTimer(
			ProjectileLifetimeHandle, // handle to cancel timer at a later time
			this, // the owning object
			&AProjectileBase::DestroyProjectile, // function to call on elapsed
			ProjectileData->GetProjectileLifeTime(), // float delay until elapsed
			false);// Make the timer loop
		
		StartGroundCheck();
	}
}

void AProjectileBase::SpawnAdditionalProjectile()
{
	if (GetOwner() != nullptr && ProjectileData != nullptr)
	{
		UProjectileData* AdditionalProjectile = ProjectileData->GetProjectileOnImpact();
		if (AdditionalProjectile != nullptr)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AProjectileBase* SpawnedProj = GetWorld()->SpawnActor<AProjectileBase>(GetClass(),GetActorLocation(),GetActorRotation(),SpawnParams);
			SpawnedProj->SetOwner(GetOwner());
			SpawnedProj->SetProjectileProperties(AdditionalProjectile,true);
		}
	}
}

void AProjectileBase::DestroyProjectile()
{
	GetWorld()->GetTimerManager().ClearTimer(ProjectileLifetimeHandle);
	Destroy();
}

void AProjectileBase::ApplyEffectToTarget(AActor* Target, bool bIsPlayer)
{
	if (ProjectileData == nullptr)
		return;
		
	if (bIsPlayer && PlayerResponse == EFactionHitType::Damage ||
		!bIsPlayer && EnemyResponse == EFactionHitType::Damage)
	{
		FGameplayEventData Payload;
		Payload.Target = Target;
		Payload.OptionalObject = ProjectileData;
		Payload.EventMagnitude = HitInfoIndex;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner,TAG_Event_Combat_ProjectileHit,Payload);
	}
	
	if (bIsPlayer && PlayerResponse != EFactionHitType::None ||
		!bIsPlayer && EnemyResponse != EFactionHitType::None)
	{
		TSubclassOf<UGameplayEffect> AddEffect = ProjectileData->GetEffectOnHit(bIsPlayer);
	
		if (AddEffect == nullptr)
			return;
		
		IAbilitySystemInterface* ImplementedOwner = Cast<IAbilitySystemInterface>(Target);
		UAbilitySystemComponent* TargetASC = nullptr;
		if (ImplementedOwner)
		{
			TargetASC = ImplementedOwner->GetAbilitySystemComponent();
			if (TargetASC == nullptr)
				return;
		}
		else
		{
			return;
		}
		FGameplayEffectContextHandle Context = OwnerAbilitySystem->MakeEffectContext();
		OwnerAbilitySystem->BP_ApplyGameplayEffectToTarget(AddEffect,TargetASC,1,Context);
	}
	
	
}

void AProjectileBase::GroundCheck()
{
	FVector Location = GetActorLocation();
			
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Init(GetOwner(),1);
	FHitResult result;
	
	FCollisionObjectQueryParams ColObjParams;
	ColObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
			
	bool bDidHit = GetWorld()->SweepSingleByObjectType(result,Location,Location,FQuat::Identity,ColObjParams,FCollisionShape::MakeSphere(ProjectileSphere->GetScaledSphereRadius()));
			
	if (bDidHit)
	{
		Land();
		if (ProjectileMoveComponent->ProjectileGravityScale == 0.0f)
		{
			FVector Normal = result.ImpactNormal;
			FRotator Rotation = FRotationMatrix::MakeFromZ(Normal).Rotator();
			SetActorRotation(Rotation);
			SetActorLocation(result.ImpactPoint + Normal * 2.0f);
		}
	}
}

void AProjectileBase::ProjectilePulse()
{
	FVector Location = GetActorLocation();
			
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Init(GetOwner(),1);
	FHitResult result;
	
	FCollisionObjectQueryParams ColObjParams;
	
	if (PlayerResponse != EFactionHitType::None)
		ColObjParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);
	if (EnemyResponse != EFactionHitType::None)
		ColObjParams.AddObjectTypesToQuery(ECC_Pawn);
	
	UE_LOG(LogTemp,Display,TEXT("Pulse loop being called"));
			
	bool bDidHit = GetWorld()->SweepSingleByObjectType(result,Location,Location,FQuat::Identity,ColObjParams,FCollisionShape::MakeSphere(ProjectileSphere->GetScaledSphereRadius()));
	
	if (bDidHit)
		OnProjectileImpact(result);
}

void AProjectileBase::StartGroundCheck()
{
	if (!GroundCheckHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(
			GroundCheckHandle, // handle to cancel timer at a later time
			this, // the owning object
			&AProjectileBase::GroundCheck, // function to call on elapsed
			GroundCheckFrequency, // float delay until elapsed
			true);// Make the timer loop
	}
}

void AProjectileBase::Land()
{
	if (ProjectileData->GetImpactPolicy() == EImpactPolicy::Detonate)
	{
		SpawnAdditionalProjectile();
		DestroyProjectile();
	}
	else if (ProjectileData->GetImpactPolicy() == EImpactPolicy::StickySurface)
	{
		ProjectileMoveComponent->ProjectileGravityScale = 0.0f;
		ProjectileMoveComponent->Velocity = FVector::ZeroVector;
		
		if (!PulseHandle.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(
			PulseHandle, // handle to cancel timer at a later time
			this, // the owning object
			&AProjectileBase::ProjectilePulse, // function to call on elapsed
			ProjectileData->GetPulseFrequency(), // float delay until elapsed
			true);
		}
		
	}
}

void AProjectileBase::OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnProjectileImpact(SweepResult);
}

void AProjectileBase::OnProjectileImpact(FHitResult HitResult)
{
	if (ProjectileData != nullptr)
	{
		AActor* OtherActor = HitResult.GetActor();
		if (OtherActor->ActorHasTag("Enemy"))
		{
			ApplyEffectToTarget(OtherActor,false);
		}
		else if (OtherActor->ActorHasTag("Player"))
		{
			ApplyEffectToTarget(OtherActor,true);
		}
		
		if (ProjectileData->GetImpactPolicy() == EImpactPolicy::Detonate)
		{
			SpawnAdditionalProjectile();
			DestroyProjectile();
		}
		else if (ProjectileData->GetImpactPolicy() == EImpactPolicy::StickySurface)
		{
			Land();
		}
	}
}

