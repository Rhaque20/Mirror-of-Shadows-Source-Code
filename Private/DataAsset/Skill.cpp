// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/Skill.h"

UAnimMontage* USkill::GetAnimation(int index) const
{
    if (index >= AnimationMontages.Num())
        return nullptr;
    
    return AnimationMontages[index];
}
