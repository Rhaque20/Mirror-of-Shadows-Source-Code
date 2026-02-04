// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/SkillHitDataStruct.h"
#include "Enumerator/FactionHitType.h"

#include "ProjectileData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EImpactPolicy : uint8
{
	None UMETA(DisplayName="None"),
	Detonate UMETA(DisplayName="Detonate", ToolTip="Destroy upon hitting a wall"),
	StickySurface UMETA(DisplayName="Sticky", ToolTip = "Sticks to the surface upon hit"),
	BounceSurface UMETA(DisplayName = "Bouncy", ToolTip = "Bounces a set number of times on impact")
};

UCLASS(Blueprintable)
class MIRROROFSHADOWS_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()
	protected:
		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
		FSkillHitData SkillHitData;
	
		UPROPERTY(Category="Projectile Physics",BLueprintReadOnly, EditDefaultsOnly, meta = (ToolTip ="Use sphere cast. If false, uses Capsule instead"))
		bool bUsesSphere = true;
	
		UPROPERTY(Category="Projectile Physics",BLueprintReadOnly, EditDefaultsOnly, meta = (ToolTip ="Determines what to do upon impact"))
		EImpactPolicy ImpactPolicy = EImpactPolicy::Detonate;
	
	
		UPROPERTY(Category="Projectile Physics",BLueprintReadOnly, EditDefaultsOnly, meta = (ToolTip ="Determines if bounce or stick also applies to entities", 
			EditCondition = "ImpactPolicy == EImpactPolicy::StickySurface || ImpactPolicy == EImpactPolicy::BounceSurface",EditConditionHides))
		bool bAffectsEntities;
		
		UPROPERTY(Category = "Projectile Sticky", BlueprintReadOnly,EditDefaultsOnly, 
			meta = (ToolTip ="Determines how many times the projectile can hit a target while stuck before destroying itself. -1 is infinite and will last until projectile lifetime ends.", 
			EditCondition = "ImpactPolicy == EImpactPolicy::StickySurface",EditConditionHides))
		int ProjectileUses = -1;
		
		UPROPERTY(Category = "Projectile Sticky", BlueprintReadOnly,EditDefaultsOnly, 
		meta = (ToolTip ="Determines how frequently the projectile will attempt to deal damage.", 
		EditCondition = "ImpactPolicy == EImpactPolicy::StickySurface",EditConditionHides, ClampMin = "0.01"))
		float PulseFrequency = 0.5f;
	
	
		UPROPERTY(Category="Projectile Physics",BlueprintReadOnly,EditDefaultsOnly)
		bool bAffectedByGravity = false;
		
		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
		bool bSizeOverLifeTime = false;
	
		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
		float ProjectileSpeed = 300.0f;
		
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (EditCondition = "bSizeOverLifeTime", EditConditionHides))
		FVector2D RadiusRange = FVector2D(32.0f,64.0f);
		
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (EditCondition = "!bSizeOverLifeTime", EditConditionHides))
		float HitRadius = 32.0f;
	
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Visual Data")
		class UNiagaraSystem* ProjectileVFX;
	
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Visual Data")
		class UNiagaraSystem* ExplosionVFX;
	
	
	
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ProjectileEffect")
		bool bDifferentEffectForFaction = false;
	
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "ProjectileEffect", meta = (EditCondition = "!bDifferentEffectForFaction", EditConditionHides))
		TSubclassOf<class UGameplayEffect> EffectOnHit;
	
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "ProjectileEffect", meta = (EditCondition = "bDifferentEffectForFaction", EditConditionHides))
		TSubclassOf<class UGameplayEffect> PlayerEffectOnHit;
	
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Category = "ProjectileEffect", meta = (EditCondition = "bDifferentEffectForFaction", EditConditionHides))
		TSubclassOf<class UGameplayEffect> EnemyEffectOnHit;
	
	
	
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TSubclassOf<UGameplayEffect> DamageEffect;
	
		UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
		UProjectileData* ProjectileOnImpact;
	
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float ProjectileLifeTime = 3.0f;
		
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Hit Target")
		EFactionHitType PlayerResponse = EFactionHitType::None;
		
		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Hit Target")
		EFactionHitType EnemyResponse = EFactionHitType::None;
	
	public:
		UNiagaraSystem* GetProjectileVFX() const
		{
			return ProjectileVFX;
		}
		
		UNiagaraSystem* GetExplosionVFX() const
		{
			return ExplosionVFX;
		}
	
		TSubclassOf<UGameplayEffect> GetEffectOnHit(bool bIsPlayer = true) const
		{
			if (bDifferentEffectForFaction)
			{
				if (bIsPlayer)
					return PlayerEffectOnHit;
				
				return EnemyEffectOnHit;
			}
			
			return EffectOnHit;
		}
	
		TSubclassOf<UGameplayEffect> GetDamageEffect() const
		{
			return DamageEffect;
		}
	
		void GetSphereHitRadius(bool& bOutSizeChange, float& minRadius, float& maxRadius) const
		{
			bOutSizeChange = bSizeOverLifeTime;
			if (bSizeOverLifeTime)
			{
				minRadius = RadiusRange.X;
				maxRadius = RadiusRange.Y;
			}
			else
			{
				minRadius = HitRadius;
				maxRadius = HitRadius;
			}
		}
	
		float GetProjectileSpeed() const
		{
			return ProjectileSpeed;
		}
	
		UProjectileData* GetProjectileOnImpact() const
		{
			return ProjectileOnImpact;
		}
	
		bool GetAffectedByGravity() const
		{
			return bAffectedByGravity;
		}
	
		EImpactPolicy GetImpactPolicy() const
		{
			return ImpactPolicy;
		}

		FSkillHitData ReturnSkillHitData() const
		{
			return SkillHitData;
		}
		
		float GetProjectileLifeTime() const
		{
			return ProjectileLifeTime;
		}
	
		EFactionHitType GetPlayerResponse() const
		{
			return PlayerResponse;
		}
		
		EFactionHitType GetEnemyResponse() const
		{
			return EnemyResponse;
		}
		
		int GetProjectileUses() const
		{
			return ProjectileUses;
		}
		
		float GetPulseFrequency() const
		{
			return PulseFrequency;
		}
};
