// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCore.h"
#include "DataAsset/PlayerSkill.h"
#include "Containers/Array.h"
#include "AbilitySystemComponent.h"
#include "PlayerCharacters.h"

#include "Kismet/KismetSystemLibrary.h"

#define PlayerChannel ETraceTypeQuery::TraceTypeQuery13
#define EnemyChannel ETraceTypeQuery::TraceTypeQuery14


UPlayerCore::UPlayerCore()
{
    OwningPlayer = Cast<APlayerCharacters>(GetOwner());
}

void UPlayerCore::NormalAttack()
{
    if (!bIsAttacking)
    {
        SetUpAttackAnim();
    }
    else if (CanBuffer)
    {
        HasBuffer = true;
    }
    
}

void UPlayerCore::Hit()
{
    /*FVector Location = GetOwner()->GetActorLocation();
    FVector ForwardVector = GetOwner()->GetActorForwardVector() * 50.f;
    FRotator Rotation = GetOwner()->GetActorRotation();

    const FVector BoxExtents = FVector(32,32,32) * NormalAttacks[CurrentChain]->GetHitBoxScale();
    TArray<AActor*> IgnoreActor;
    IgnoreActor.Init(GetOwner(),1);
    TArray<FHitResult> Results;



    UKismetSystemLibrary::BoxTraceMulti(this,Location + ForwardVector, Location + ForwardVector + FVector(0.1f,0.1f,0.1f)
    ,BoxExtents,Rotation,EnemyChannel,false,IgnoreActor ,EDrawDebugTrace::Type::ForDuration,Results,true);

    InflictDamage(Results);*/
}

void UPlayerCore::SetUpAttackAnim()
{
    bIsAttacking = true;

    UAnimMontage* AttackAnim = NormalAttacks[CurrentChain]->GetAnimation(0);
    if (AttackAnim != nullptr)
    {
        SkeletalMesh->GetAnimInstance()->Montage_Play(AttackAnim,1.f,EMontagePlayReturnType::Duration,0.f,true);
        // Deal with fact of chain going beyond the array
    }
}

void UPlayerCore::SetSkillRef(int index)
{
    CurrentChain = index;
}

void UPlayerCore::GiveNewSkill(UPlayerSkill* NewSkill)
{
    ESkillHeight SkillHeight = NewSkill->GetSkillHeightRequirement();
    if (SkillHeight == ESkillHeight::Grounded || SkillHeight == ESkillHeight::GroundorAir)
    {
        GroundSkills.Add(NewSkill);
    }
    else
    {
        AirSkills.Add(NewSkill);
    }
}

void UPlayerCore::FillAllSkillEnergy(float EnergyAmount)
{
    int n = GroundSkills.Num();
    int i = 0;
    for (i = 0; i < n; i++)
    {
        GroundSkills[i]->FillSkillEnergy(EnergyAmount);
    }

    n = AirSkills.Num();

    for (i = 0; i < n; i++)
    {
        AirSkills[i]->FillSkillEnergy(EnergyAmount);
    }
}

void UPlayerCore::FillAllSkillEnergy(float EnergyAmount, FGameplayTag SkillElement, float MatchModifier, float MismatchModifier)
{
    int n = GroundSkills.Num();
    int i = 0;
    for (i = 0; i < n; i++)
    {
        GroundSkills[i]->FillSkillEnergy(EnergyAmount * (GroundSkills[i]->GetSkillElement() == SkillElement ? MatchModifier : MismatchModifier));
    }

    n = AirSkills.Num();

    for (i = 0; i < n; i++)
    {
        AirSkills[i]->FillSkillEnergy(EnergyAmount * (AirSkills[i]->GetSkillElement() == SkillElement ? MatchModifier : MismatchModifier));
    }
}

bool UPlayerCore::CanUseSkill(int index, bool bOnGround) const
{
    if (index < 0)
        return false;

    if (bOnGround)
    {
        if (index < GroundSkills.Num())
        {
            return GroundSkills[index]->IsReadyToUse();
        }
    }
    else
    {
        if (index < AirSkills.Num())
        {
            return AirSkills[index]->IsReadyToUse();
        }
    }

    return false;
}

void UPlayerCore::GetSkillEnergyAtIndex(int Index,float& EnergyRatio, float& CurrentEnergy, float& MaxEnergy, bool OnGround) const
{
    if (Index > 0)
    {
        if (OnGround)
        {
            if (Index < GroundSkills.Num())
            {
                GroundSkills[Index]->GetSkillEnergyInfo(CurrentEnergy, MaxEnergy);
                EnergyRatio = CurrentEnergy / MaxEnergy;
                return;
            }
        }
        else
        {
            if (Index < AirSkills.Num())
            {
                AirSkills[Index]->GetSkillEnergyInfo(CurrentEnergy, MaxEnergy);
                EnergyRatio = CurrentEnergy / MaxEnergy;
                return;
            }
        }
    }

    EnergyRatio = 0.0f;
    CurrentEnergy = 0.0f;
    MaxEnergy = 0.0f;

}

