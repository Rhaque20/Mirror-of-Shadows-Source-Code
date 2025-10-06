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

/// <summary>
/// Called at begin play, keeps track of the active members in the party
/// as well as bind any necessary functions to them.
/// </summary>
/// <param name="PartyList">Already spawned players that can be immediately used</param>
void APlayerPartyController::SetUpMembers(TArray<APlayerCharacters*> PartyList)
{
    SummonedActorReferences = PartyList;
    AliveMembersRemaining = SummonedActorReferences.Num();
    ActiveCharacter = SummonedActorReferences[0];

    for (int i = 0; i < AliveMembersRemaining; i++)
    {
        // Controller ref exists to keep track of the party data such as skill chain level
        // and skill chain element.
        SummonedActorReferences[i]->GiveControllerRef(this);
        SummonedActorReferences[i]->SkillActivated.AddDynamic(this,&APlayerPartyController::GiveSPToOtherAllies);
    }
}

/// <summary>
/// Called by GA_GenericPlayerSkill and its children (I know this breaks encapsulation)
/// Will increment the chain level and set the element of the chain level.
/// This gets called during a skill's recovery giving time for the player to increment
/// it for skills with long animations.
/// </summary>
/// <param name="SkillElement">Element of the skill chain to trigger</param>
void APlayerPartyController::IncreaseSkillChainLevel(EElement SkillElement = EElement::Physical)
{
    // Original chain level
    int oldChain = SkillChainLevel;
    if (SkillChainLevel == 3)
    {
        // While visually it progresses from 3 to 4, mechanically, the player can trigger skill chain 3 twice
        if (bExtendChainThree)
        {
            SkillChainAdvance.Broadcast(4, SkillElement);
            bExtendChainThree = false;
        }  
        else
        {
            // If we have applied both chances of skill chain 3 effects, end the chain.
            EndChainTimer();
        }
            
    }
    else
    {
        // In normal cases, incrememnt the skill chain and broadcast the new element and chain
        // to whomever is subscribed to it.
        SkillChainLevel++;
        SkillChainAdvance.Broadcast(SkillChainLevel, SkillElement);
    }

    UE_LOG(LogTemp, Display, TEXT("Skill Chain Increased from %d to %d"),oldChain,SkillChainLevel);
    // The chain timer begins here.
    StartChainTimer();
}

/// <summary>
/// Used for pausing or playing the skill chain timer. This is done when activating another skill
/// to the chain.
/// </summary>
/// <param name="bTimerIsPlaying">Bool to determine to play or pause</param>
void APlayerPartyController::SetChainTimerPlaying(bool bTimerIsPlaying)
{
    bIsTimerActive = bTimerIsPlaying;
}

/// <summary>
/// Called at beginning of gameplay, sets up the necessary parameters.
/// </summary>
void APlayerPartyController::BeginPlay()
{
    Super::BeginPlay();


    // Remove GetPawn and find another way to get current player in the world
    // Until I do so, just use this to get the active location of the spawned player
    // This being a blank pawn that exists purely to get location and rotation data.
    APawn* PawnSummoned = UGameplayStatics::GetPlayerPawn(this, 0);
    FVector CurrentPos = PawnSummoned->GetActorLocation();
    FRotator CurrentRot = PawnSummoned->GetActorRotation();

    // Spawn params to ensure party members always spawn with or without obstruction.
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    if (PartyMemberData[0] != nullptr)
    {
        // Spawn the party members through the established list
        for (int i = 0; i < PartyMemberData.Num(); i++)
        {
            // Spawn the party member based on the class specified in the data struct for party member data.
            APlayerCharacters* CurrentPlayer = GetWorld()->SpawnActor<APlayerCharacters>(PartyMemberData[i]->GetCharacterClass(), CurrentPos, CurrentRot,SpawnParams);

            // Keeping here purely for future issues that may arise. First time this happened was because of
            // obstruction of spawn point.
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
                // Hide other party members that aren't the first.
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

/// <summary>
///  Used when pressing left D-Pad or Q (adjustable controls) to summon left member
/// </summary>
void APlayerPartyController::SwapCharacterLeft()
{
    int SwapToIndex = GetCharacterLeftIndex();

    if(SwapToIndex != SWAPFAIL)
        SwapCharacter(SwapToIndex);
}

/// <summary>
/// Used when pressing right D-Pad or E (adjustable controls) to summon right member.
/// </summary>
void APlayerPartyController::SwapCharacterRight()
{
    int SwapToIndex = GetCharacterRightIndex();

    if (SwapToIndex != SWAPFAIL)
        SwapCharacter(SwapToIndex);
}

/// <summary>
/// This function exists due to negative modulos not looping to max.
/// </summary>
/// <returns></returns>
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

    // Go back on the index to determine who to swap to and adjust accordingly.
    int SwapToIndex = CurrentCharacterIndex - 1;
    if (SwapToIndex < 0) SwapToIndex = AliveMembersRemaining - 1;

    return SwapToIndex;
}

/// <summary>
/// Simple get next member in the array.
/// </summary>
/// <returns></returns>
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

/// <summary>
/// Perform a summon attack instead of switching
/// </summary>
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

/// <summary>
/// Same with above function. Perform summon attack instead of switching.
/// </summary>
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

/// <summary>
/// Tick function for chain timer to decrement as the timer is going if the timer is playing.
/// </summary>
void APlayerPartyController::ChainTimerUpdate()
{
    if (bIsTimerActive)
    {
        // Use the interval to decrement the chain time.
        RemainingChainTime -= ChainTimerInterval;

        // Broadcast the ratio to anyone that is subscribed
        SkillChainTimeUpdate.Broadcast(RemainingChainTime / MaxChainTime);

        if (RemainingChainTime <= 0.0f)
        {
            EndChainTimer();
        }
    }
}

/// <summary>
/// Used to activate the chain timer.
/// </summary>
void APlayerPartyController::StartChainTimer()
{
    RemainingChainTime = MaxChainTime;
    if (!ChainTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().SetTimer(
            ChainTimerHandle, // handle to cancel timer at a later time
            this, // the owning object
            &APlayerPartyController::ChainTimerUpdate, // function to call on elapsed
            ChainTimerInterval, // float delay until elapsed
            true);// Make the timer loop
    }

}

/// <summary>
/// Called either from the tick function or from the chain incrementer.
/// Ends the chain combo and set the level to 0.
/// </summary>
void APlayerPartyController::EndChainTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(ChainTimerHandle);
    SkillChainLevel = 0;
    SkillChainAdvance.Broadcast(SkillChainLevel, EElement::Physical);
}

/// <summary>
/// Used to handle summon attack which is when a party member (unpossessed) performs
/// a skill they bound to their QTE (Quick Time Event) skill.
/// </summary>
/// <param name="index">Party Member index to summon attack</param>
void APlayerPartyController::SummonAttack(int index)
{
    APlayerCharacters* SwapToPlayer = SummonedActorReferences[index];

    // If we have a lock on target, set the summoned ally to be within summon range of the lockon target
    if (LockOnTarget)
    {
        FVector LocationVec = LockOnTarget->GetActorLocation() + (SummonRadius * LockOnTarget->GetActorForwardVector());
        SwapToPlayer->SetActorLocation(LocationVec);
    }
    else
    {
        // Else place the summoned ally at the active player's location
        SwapToPlayer->SetActorLocation(ActiveCharacter->GetActorLocation());
    }

    // Have them face the same as the active player's rotation
    // This will get overriden if the player has a lock on target.
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
/// Switch to another character
/// </summary>
/// <param name="SwapToIndex">Index to swap to</param>
void APlayerPartyController::SwapCharacter(int SwapToIndex) 
{
    APlayerCharacters* SwapToPlayer = SummonedActorReferences[SwapToIndex];

    // If the ally is in the middle of a skill, ignore the swap request.
    if (SwapToPlayer->GetIsSummoned())
        return;

    FVector CurrentPos = ActiveCharacter->GetActorLocation();
    FRotator CurrentRot = ActiveCharacter->GetActorRotation();

    // Get the rotation of the camera relative to the player.
    FRotator CameraRot = ActiveCharacter->GetController()->GetControlRotation();

    Possess(Cast<APawn>(SwapToPlayer));

    // Setting up the parameters to make it match exactly the original location, rotation
    // and camera rotation as the previously active character.
    SwapToPlayer->SetActorLocation(CurrentPos);
    SwapToPlayer->SetActorRotation(CurrentRot);

    SwapToPlayer->GetController()->SetControlRotation(CameraRot);
    
     UE_LOG(LogTemp, Display, TEXT("Swapped party members and relocated camera"));

     // Make the swap to player visible.
    SwapToPlayer->SetPlayerActive(true);
    // Rename to CanBeInterrupted

    // If the character about to be swapped out is in the middle of a skill, swap cancel out of it
    // allowing that character to finish casting their skill while also treating the
    // attack as a summoned skill
    if (ActiveCharacter->IsInSkill())
    {
        ActiveCharacter->SetIsSummoned(true);
    }
    else // Otherwise swap as normal.
        ActiveCharacter->SetPlayerActive(false);

    // Perform the necessary actions for swapping in and out.
    SwapToPlayer->SwapIn();
    ActiveCharacter->SwapOut();

    // Transfer the lock on target from the previously active character to the new one
    SwapToPlayer->SetUpLockOn(IsLockedOn,LockOnTarget);

    // Give the offield state effect to the swapped out character and remove from
    // the on field one. This effect prevents the offfield character from taking
    // damage when doing a summon attack.
    UAbilitySystemComponent *ActiveASC, *SwapToASC;
    ActiveASC = ActiveCharacter->GetAbilitySystemComponent();
    SwapToASC = SwapToPlayer->GetAbilitySystemComponent();

    if(OfffieldState)
        ActiveASC->BP_ApplyGameplayEffectToSelf(OfffieldState,0,ActiveASC->MakeEffectContext());
    else
        UE_LOG(LogTemp, Error, TEXT("OFFFIELD STATE EFFECT IS NULL IN PARTY CONTROLLER"));

    SwapToASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_Effect_State_OffField));
    // Unpause the chain timer when swapping out
    // This is used to avoid abusing the timer freeze from skill commitments without the commitment.
    bIsTimerActive = true;


    // Reference switching
    ActiveCharacter = SwapToPlayer;
    CurrentCharacterIndex = SwapToIndex;
}

/// <summary>
/// Used for skills that grant effects (buff/debuff) to all allies (since they're not on the field)
/// </summary>
/// <param name="Handle">Effect to give to all allies</param>
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

/// <summary>
/// Removes an effect from all allies (same deal)
/// </summary>
/// <param name="RemovalTag"></param>
void APlayerPartyController::RemoveGameplayEffectForAllAllies(FGameplayTag RemovalTag)
{

    for (APlayerCharacters* Member : SummonedActorReferences)
    {
        UE_LOG(LogTemp, Display, TEXT("%s has their buff removed"), *(Member->GetName()));
        Member->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(RemovalTag));
    }
}

/// <summary>
/// Used for SP recycling by granting all allies (including the caster) a portion of the SP consumed
/// to all of their skills, except for the skill that was activated.
/// The amount of energy given is based on if the skill receiving the energy matches the element of the activated skill
/// (Ex. Light Skills give more energy to other light skills and normal amount to other element skills)
/// </summary>
/// <param name="ActivatingActor">Who activated the skill?</param>
/// <param name="SkillActivated">What skill was activated?</param>
void APlayerPartyController::GiveSPToOtherAllies(APlayerCharacters* ActivatingActor, UPlayerSkill* SkillActivated)
{
    float EnergyRemaining, EnergyCost;
    SkillActivated->GetSkillEnergyInfo(EnergyRemaining, EnergyCost);

    // The amount of energy to give is tied to how much energy was consumed
    float SkillEnergyToGive = EnergyCost - EnergyRemaining;
    // Get the element of the skill
    FGameplayTag SkillElement = SkillActivated->GetSkillElement();
    // Prevent the activating skill from receiving it.
    SkillActivated->SetCanGainSkillEnergy(false);
    
    // Go through all the members (including the caster) and give the energy.
    for (APlayerCharacters* Member : SummonedActorReferences)
    {
        Member->GainSkillEnergy(SkillEnergyToGive, SkillElement,EnergyRefundRatio,EnergyRefundRatioMismatch, Member == ActivatingActor);
    }

    // Re-enable energy gain.
    SkillActivated->SetCanGainSkillEnergy(true);
}




