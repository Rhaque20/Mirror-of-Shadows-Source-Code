// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPartyController.h"
#include "DataAsset/PlayerData.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacters.h"
#include "Components/PrimitiveComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "GAS/UDGameplayTags.h"
#include "DataAsset/PlayerSkill.h"

const int SWAPFAIL = -1;


APlayerPartyController::APlayerPartyController()
{
    
}

void APlayerPartyController::SetUpMembers(TArray<APlayerCharacters*> PartyList)
{
    SummonedActorReferences = PartyList;
    AliveMembersRemaining = SummonedActorReferences.Num();
    ActiveCharacter = SummonedActorReferences[0];

    for (int i = 0; i < AliveMembersRemaining; i++)
    {
        SummonedActorReferences[i]->GiveControllerRef(this);
        SummonedActorReferences[i]->SkillActivated.AddDynamic(this,&APlayerPartyController::GiveSPToOtherAllies);
    }
}

void APlayerPartyController::IncreaseSkillChainLevel(EElement SkillElement = EElement::Physical)
{
    int oldChain = SkillChainLevel;
    if (SkillChainLevel == 3)
    {
        if (bExtendChainThree)
        {
            SkillChainAdvance.Broadcast(4, SkillElement);
            bExtendChainThree = false;
        }  
        else
        {
            EndChainTimer();
        }
            
    }
    else
    {
        SkillChainLevel++;
        SkillChainAdvance.Broadcast(SkillChainLevel, SkillElement);
    }

    UE_LOG(LogTemp, Display, TEXT("Skill Chain Increased from %d to %d"),oldChain,SkillChainLevel);
    StartChainTimer();
}

void APlayerPartyController::SetChainTimerPlaying(bool bTimerIsPlaying)
{
    bIsTimerActive = bTimerIsPlaying;
}

void APlayerPartyController::BeginPlay()
{
    Super::BeginPlay();


    // Remove GetPawn and find another way to get current player in the world
    APawn* PawnSummoned = UGameplayStatics::GetPlayerPawn(this, 0);
    FVector CurrentPos = PawnSummoned->GetActorLocation();
    FRotator CurrentRot = PawnSummoned->GetActorRotation();
    if (PartyMemberData[0] != nullptr)
    {
        for (int i = 0; i < PartyMemberData.Num(); i++)
        {
            APlayerCharacters* CurrentPlayer = GetWorld()->SpawnActor<APlayerCharacters>(PartyMemberData[i]->GetCharacterClass(), CurrentPos, CurrentRot);
            if (!CurrentPlayer)
            {
                UE_LOG(LogTemp, Error, TEXT("How the hell did this end up being null?"));
                continue;
            }
                
            if (i == 0)
            {
                Possess(Cast<APawn>(CurrentPlayer));
            }
            else
            {
                CurrentPlayer->SetPlayerActive(false);
            }
            SummonedActorReferences.Add(CurrentPlayer);
        }

        SetUpMembers(SummonedActorReferences);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Party Member not initialized or controller is null"));
    }
    
}

void APlayerPartyController::SwapCharacterLeft()
{
    int SwapToIndex = GetCharacterLeftIndex();

    if(SwapToIndex != SWAPFAIL)
        SwapCharacter(SwapToIndex);
}

void APlayerPartyController::SwapCharacterRight()
{
    int SwapToIndex = GetCharacterLeftIndex();

    if (SwapToIndex != SWAPFAIL)
        SwapCharacter(SwapToIndex);
}

int APlayerPartyController::GetCharacterLeftIndex()
{
    // Edit this accordingly once we deal with certain indices being unavailable
    if (AliveMembersRemaining == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Party Member TArray is empty"));
        return SWAPFAIL;
    }
    else if (AliveMembersRemaining == 1)
    {
        UE_LOG(LogTemp, Display, TEXT("No members to swap to"));
        return SWAPFAIL;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Party Member Array count is %d"), AliveMembersRemaining);
    }

    int SwapToIndex = CurrentCharacterIndex - 1;
    if (SwapToIndex < 0) SwapToIndex = AliveMembersRemaining - 1;

    return SwapToIndex;
}

int APlayerPartyController::GetCharacterRightIndex()
{
    if (AliveMembersRemaining == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Party Member TArray is empty"));
        return SWAPFAIL;
    }
    else if (AliveMembersRemaining == 1)
    {
        UE_LOG(LogTemp, Display, TEXT("No members to swap to"));
        return SWAPFAIL;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Party Member Array count is %d"), AliveMembersRemaining);
    }

    return (CurrentCharacterIndex + 1) % SummonedActorReferences.Num();
}

void APlayerPartyController::SummonAttackLeft()
{
    int SwapToIndex = GetCharacterLeftIndex();

    if (SwapToIndex != SWAPFAIL)
    {
        SummonAttack(SwapToIndex);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Swap Skill failed on PlayerPartyController"));
    }
}

void APlayerPartyController::SummonAttackRight()
{
    int SwapToIndex = GetCharacterRightIndex();

    if (SwapToIndex != SWAPFAIL)
    {
        SummonAttack(SwapToIndex);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Swap Skill failed on PlayerPartyController"));
    }
}

void APlayerPartyController::ChainTimerUpdate()
{
    if (bIsTimerActive)
    {
        RemainingChainTime -= GetWorld()->GetDeltaSeconds();

        SkillChainTimeUpdate.Broadcast(RemainingChainTime / MaxChainTime);

        if (RemainingChainTime <= 0.0f)
        {
            EndChainTimer();
        }
    }
}

void APlayerPartyController::StartChainTimer()
{
    RemainingChainTime = MaxChainTime;
    if (!ChainTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().SetTimer(
            ChainTimerHandle, // handle to cancel timer at a later time
            this, // the owning object
            &APlayerPartyController::ChainTimerUpdate, // function to call on elapsed
            GetWorld()->GetDeltaSeconds(), // float delay until elapsed
            true);
    }

}

void APlayerPartyController::EndChainTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(ChainTimerHandle);
    SkillChainLevel = 0;
    SkillChainAdvance.Broadcast(SkillChainLevel, EElement::Physical);
}

void APlayerPartyController::SummonAttack(int index)
{
    APlayerCharacters* SwapToPlayer = SummonedActorReferences[index];
    if (!IsValid(LockOnTarget))
    {
        UE_LOG(LogTemp, Display, TEXT("Invalid lockon feed"));
    }

    if (LockOnTarget)
    {
        FVector activeCharLoc = ActiveCharacter->GetActorLocation();
        FVector LocationVec = LockOnTarget->GetActorLocation() + (SummonRadius * LockOnTarget->GetActorForwardVector());
        SwapToPlayer->SetActorLocation(LocationVec);
    }
    else
    {
        SwapToPlayer->SetActorLocation(ActiveCharacter->GetActorLocation());
    }

    SwapToPlayer->SetActorRotation(ActiveCharacter->GetActorRotation());

    if (SwapToPlayer->SummonAttack(LockOnTarget,ActiveCharacter->IsOnGround()))
    {
        SwapToPlayer->SetPlayerActive(true);
        UE_LOG(LogTemp, Display, TEXT("Summon attack successful"));
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Summon attack unsuccessful"));
    }
}

/// <summary>
/// 
/// </summary>
/// <param name="SwapToIndex"></param>
void APlayerPartyController::SwapCharacter(int SwapToIndex) 
{
    APlayerCharacters* SwapToPlayer = SummonedActorReferences[SwapToIndex];

    if (SwapToPlayer->GetIsSummoned())
        return;

    if (!IsValid(ActiveCharacter))
    {
        UE_LOG(LogTemp, Error, TEXT("WE SOMEHOW LOST THE ORIGINAL ACTIVE CHARACTER AND HAVE TO USE BACKUP"));
        ActiveCharacter = SummonedActorReferences[CurrentCharacterIndex];
    }

    FVector CurrentPos = ActiveCharacter->GetActorLocation();
    FRotator CurrentRot = ActiveCharacter->GetActorRotation();

    FRotator CameraRot = ActiveCharacter->GetController()->GetControlRotation();

    Possess(Cast<APawn>(SwapToPlayer));

    SwapToPlayer->SetActorLocation(CurrentPos);
    SwapToPlayer->SetActorRotation(CurrentRot);

    SwapToPlayer->GetController()->SetControlRotation(CameraRot);
    
     UE_LOG(LogTemp, Display, TEXT("Swapped party members and relocated camera"));

    // // Note camera rotation needs to be adjusted on the character swap
    // // Also create onswitch event to ensure normal attacks can still work.

    SwapToPlayer->SetPlayerActive(true);
    // Rename to CanBeInterrupted
    if (ActiveCharacter->IsInSkill())
    {
        ActiveCharacter->SetIsSummoned(true);
    }
    else
        ActiveCharacter->SetPlayerActive(false);

    SwapToPlayer->SwapIn();
    ActiveCharacter->SwapOut();
    SwapToPlayer->SetUpLockOn(IsLockedOn,LockOnTarget);

    UAbilitySystemComponent *ActiveASC, *SwapToASC;
    ActiveASC = ActiveCharacter->GetAbilitySystemComponent();
    SwapToASC = SwapToPlayer->GetAbilitySystemComponent();

    if(OfffieldState)
        ActiveASC->BP_ApplyGameplayEffectToSelf(OfffieldState,0,ActiveASC->MakeEffectContext());
    else
        UE_LOG(LogTemp, Error, TEXT("OFFFIELD STATE EFFECT IS NULL IN PARTY CONTROLLER"));

    SwapToASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_Effect_State_OffField));
    bIsTimerActive = true;


    // Reference switching
    ActiveCharacter = SwapToPlayer;
    CurrentCharacterIndex = SwapToIndex;
}

void APlayerPartyController::ApplyGameplayEffectToAllAllies(FGameplayEffectSpecHandle Handle)
{
    if (Handle.IsValid())
    {
        UE_LOG(LogTemp, Display, TEXT("Handle is valid"));
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Invalid Handle received"));
        return;
    }

    for (APlayerCharacters* Member : SummonedActorReferences)
    {
        UE_LOG(LogTemp, Display, TEXT("%s is receiving a buff"),*(Member->GetName()));
        Member->GetAbilitySystemComponent()->BP_ApplyGameplayEffectSpecToSelf(Handle);
    }
}

void APlayerPartyController::RemoveGameplayEffectForAllAllies(FGameplayTag RemovalTag)
{

    for (APlayerCharacters* Member : SummonedActorReferences)
    {
        UE_LOG(LogTemp, Display, TEXT("%s has their buff removed"), *(Member->GetName()));
        Member->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(RemovalTag));
    }
}

void APlayerPartyController::GiveSPToOtherAllies(APlayerCharacters* ActivatingActor, UPlayerSkill* SkillActivated)
{
    float EnergyRemaining, EnergyCost;
    SkillActivated->GetSkillEnergyInfo(EnergyRemaining, EnergyCost);
    float SkillEnergyToGive = EnergyCost - EnergyRemaining;
    FGameplayTag SkillElement = SkillActivated->GetSkillElement();
    SkillActivated->SetCanGainSkillEnergy(false);
    
    for (APlayerCharacters* Member : SummonedActorReferences)
    {
        Member->GainSkillEnergy(SkillEnergyToGive, SkillElement,EnergyRefundRatio,EnergyRefundRatioMismatch, Member == ActivatingActor);
    }

    SkillActivated->SetCanGainSkillEnergy(true);
}




