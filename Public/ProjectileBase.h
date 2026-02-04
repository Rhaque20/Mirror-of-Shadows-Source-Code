// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Enumerator/FactionHitType.h"

#include "ProjectileBase.generated.h"

UCLASS()
class MIRROROFSHADOWS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void SetProjectileProperties(class UProjectileData* ProjectileInfo,bool bStartLifeTime = true);
	UFUNCTION(BlueprintCallable)
	void FireProjectile(FVector Direction, FVector StartPos);
	UFUNCTION()
	void SpawnAdditionalProjectile();
	UFUNCTION()
	void DestroyProjectile();
	UFUNCTION()
	void ApplyEffectToTarget(AActor* Target, bool bIsPlayer);
	UFUNCTION()
	void GroundCheck();
	
	UFUNCTION()
	void ProjectilePulse();
	
	UFUNCTION()
	void StartGroundCheck();
	
	void Land();

	UFUNCTION()
	void OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	void OnProjectileImpact(FHitResult HitResult);

protected:	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile Data")
	class TSubclassOf<class UGameplayEffect> PoiseDMGEffect;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Projectile Data")
	class UProjectileData* ProjectileData;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Projectile Mesh")
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Projectile Mesh")
	class UProjectileMovementComponent* ProjectileMoveComponent;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Projectile Mesh")
	class USphereComponent* ProjectileSphere;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Projectile VFX")
	class UNiagaraComponent* ProjectileVFX;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Projectile Poise Effect")
	TSubclassOf<class UGameplayEffect> PoiseDamageEffect;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "Projectile Debug Parameters")
	float GroundCheckFrequency = 0.05f;
	
	UPROPERTY()
	USceneComponent* TestComp;
	
	UPROPERTY()
	class UAbilitySystemComponent* OwnerAbilitySystem;
	
	UPROPERTY()
	FTimerHandle ProjectileLifetimeHandle;
	
	UPROPERTY()
	FTimerHandle GroundCheckHandle;
	
	UPROPERTY()
	FTimerHandle PulseHandle;
	
	UPROPERTY()
	int HitInfoIndex = -1;
	
	UPROPERTY()
	EFactionHitType PlayerResponse = EFactionHitType::None;
	
	UPROPERTY()
	EFactionHitType EnemyResponse = EFactionHitType::None;

};
