// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGroupBehaviorManager.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GAS/UDGameplayTags.h"
#include "EnemyCharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyGroupBehaviorManager::AEnemyGroupBehaviorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyGroupBehaviorManager::BeginPlay()
{
    if (MeleeTicketEffect == nullptr)
        UE_LOG(LogTemp, Error, TEXT("MELEE TICKET EFFECT IS NULL!"));
    if (RangeTicketEffect == nullptr)
        UE_LOG(LogTemp, Error, TEXT("RANGE TICKET EFFECT IS NULL!"));

    GetWorld()->GetTimerManager().SetTimer(
        GroupLoopHandle,
        this,
        &AEnemyGroupBehaviorManager::EnemyGroupLoop,
        LoopFrequency,
        true);

	Super::BeginPlay();
	
}

/// <summary>
/// Grants a ticket to the chosen target
/// </summary>
/// <param name="Requester">Who is given the ticket</param>
/// <param name="TicketType">Whether the ticket is melee or ranged</param>
/// <returns>Was the ticket successfully given?</returns>
bool AEnemyGroupBehaviorManager::GiveTicket(AEnemyCharacterBase* Requester, FGameplayTag TicketType)
{
    UAbilitySystemComponent* ASCComponent;
    if (TicketType == TAG_EnemyAI_Ticket_Melee)
    {
        if (MeleeTickets > 0)
        {
            // Get ther ability system copmonent so the ticket effect is given.
            ASCComponent = Requester->GetAbilitySystemComponent();
            MeleeTickets--;
            // Give them their ticket to advance to the next step.
            Requester->ReceiveTicket(TicketType);
            ASCComponent->BP_ApplyGameplayEffectToSelf(MeleeTicketEffect, 1, ASCComponent->MakeEffectContext());
            // Add to the tracker to keep track of who was given the ticket.
            TicketTracker.Add(Requester, TicketType);
            return true;
        }

    }
    else if (TicketType == TAG_EnemyAI_Ticket_Range)
    {
        if (RangeTickets > 0)
        {
            ASCComponent = Requester->GetAbilitySystemComponent();
            RangeTickets--;
            ASCComponent->BP_ApplyGameplayEffectToSelf(RangeTicketEffect, 1, ASCComponent->MakeEffectContext());
            TicketTracker.Add(Requester, TicketType);
            return true;
        }
    }

    return false;
}

/// <summary>
/// Used for when receiving a ticket back from the enemy either after finishing their attack
/// or forcefully (death, skill interrupt, or ticket stolen)
/// </summary>
/// <param name="Requester">Who returned the ticket</param>
/// <param name="TicketType">What ticket was returned</param>
void AEnemyGroupBehaviorManager::ReturnTicket(AEnemyCharacterBase* Requester, FGameplayTag TicketType)
{
    if (!TicketTracker.Contains(Requester))
    {
        UE_LOG(LogTemp, Display, TEXT("Never owed a ticket"));
        return;
    }

    UAbilitySystemComponent* ASCComponent = Requester->GetAbilitySystemComponent();
        

    FGameplayTagContainer TicketToReturn = FGameplayTagContainer(TicketType);

    if (TicketType == TAG_EnemyAI_Ticket_Melee)
    {
        MeleeTickets++;
        ASCComponent->RemoveActiveEffectsWithAppliedTags(TicketToReturn);
        TicketTracker.Remove(Requester);

    }
    else if (TicketType == TAG_EnemyAI_Ticket_Range)
    {
        RangeTickets++;
        ASCComponent->RemoveActiveEffectsWithAppliedTags(TicketToReturn);
        TicketTracker.Remove(Requester);
    }
}

void AEnemyGroupBehaviorManager::StealTicket(AEnemyCharacterBase* Requester, FGameplayTag TicketType)
{
}

/// <summary>
/// Called when the grouper has spawned a new enemy. Adds the enemy to the list of enemies to manage.
/// </summary>
/// <param name="Enemy"></param>
void AEnemyGroupBehaviorManager::ReceiveEnemy(AEnemyCharacterBase* Enemy)
{
    // Adds to the enemy group list
    EnemiesInGroup.Add(Enemy);
    // Adds them to the scoreboard to be evaluated later when deciding who gets the ticket.
    EnemyScoreboard.Add(Enemy, new EnemyScore());
    // Binds their respective functions
    Enemy->OnRequestTicket.BindUFunction(this, FName("GiveTicket"));
    Enemy->OnStealTicket.BindUFunction(this, FName("StealTicket"));
    Enemy->OnReturnTicket.AddDynamic(this,&AEnemyGroupBehaviorManager::ReturnTicket);
    Enemy->CharacterDied.AddDynamic(this, &AEnemyGroupBehaviorManager::RemoveEnemy);
    if (!bDetectedPlayer)
    {
        Enemy->OnDetectedPlayer.AddDynamic(this,&AEnemyGroupBehaviorManager::DetectedPlayer);
    }
    else
    {
        Enemy->ReceivePlayerTarget(UGameplayStatics::GetPlayerCharacter(this,0));
    }
    //Add(FReturnTicketFunction::CreateUObject(this, &AEnemyGroupBehaviorManager::ReturnTicket))
}

/// <summary>
/// Called when the enemy dies
/// </summary>
/// <param name="Enemy">Enemy to remove</param>
void AEnemyGroupBehaviorManager::RemoveEnemy(ARPGCharacterBase* Enemy)
{
    AEnemyCharacterBase* EnemyRef = Cast<AEnemyCharacterBase>(Enemy);

    if (EnemiesInGroup.Contains(EnemyRef))
    {
        EnemiesInGroup.Remove(EnemyRef);
        EnemyScoreboard.Remove(EnemyRef);
    }
}

/// <summary>
/// Called by an event from the enemies to indicate one of the enemies have spotted the player.
/// </summary>
/// <param name="DetectedPlayer">Player that was spotted</param>
void AEnemyGroupBehaviorManager::DetectedPlayer(AActor* DetectedPlayer)
{
    if (bDetectedPlayer)
        return;
    
    for (AEnemyCharacterBase* Enemy : EnemiesInGroup)
    {
        Enemy->OnDetectedPlayer.RemoveAll(this);
        Enemy->ReceivePlayerTarget((DetectedPlayer));
    }
    bDetectedPlayer = true;
    OnBattleBegin.Broadcast(DetectedPlayer);
}

/// <summary>
/// Primary loop of the manager. It's loop frequency is adjustable.
/// Handles scoring of each enemy in the group to determine who gets the ticket
/// And consolating those that didn't
/// </summary>
void AEnemyGroupBehaviorManager::EnemyGroupLoop()
{
    // No tickets?
    if (MeleeTickets == 0)
        return;

    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this,0);
    FVector PlayerLocation = Player->GetActorLocation();


    // Queue to used to decide who will be given a ticket
    TArray<EnemyScore*> EnemiesQueue;
    EnemyScore* CurrentScore;

    for (AEnemyCharacterBase* Enemy : EnemiesInGroup)
    {
        // Due to struct pass by value nature, this form of redundancy is
        // to ensure the data persists.
        CurrentScore = EnemyScoreboard[Enemy];
        // Resets their distance score as that can change periodically in between the loops
        CurrentScore->ScoreForDistance = 0.0f;
        CurrentScore->EnemyRef = Enemy;

        // If the target has a ticket or is under an attack delay, reset their score bonus
        // for missed turns as they had a ticket as well as do not consider them for
        // ticket granting.
        if (Enemy->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(ResetTags))
        {
            CurrentScore->ScoreByMissedTurns = 0.0f;

            EnemyScoreboard[Enemy] = CurrentScore;
            continue;
        }

        // Determine how far they are. Using squared distance to save performance as we don't care about specifics yet.
        float DistanceScoreSqrd = UKismetMathLibrary::Vector_Distance2DSquared(PlayerLocation, Enemy->GetActorLocation());

        if (DistanceScoreSqrd < ClosenessDistanceForMelee * ClosenessDistanceForMelee)
        {
            // Formula to use to give more points the closer the target is to the player
            // Now we care about how close they are so we square root the distance and consider them
            // for giving tickets.
            CurrentScore->ScoreForDistance = ClosenessDistanceForMelee - FMath::Sqrt(DistanceScoreSqrd);
            EnemiesQueue.Add(CurrentScore);
        }
        else
        {
            CurrentScore->ScoreForDistance = 0.0f;
            EnemiesQueue.Add(CurrentScore);
        }
        // Set the changed struct.
        EnemyScoreboard[Enemy] = CurrentScore;
    }

    if (EnemiesQueue.Num() == 0)
        return;

    // Overriding the < operator for EnemyScore lets us sort them based on their combined score
    // of distance + turns missed bonus with highest score as the first element
    EnemiesQueue.Sort();

    int TicketsToRemove = 0;
    int n = EnemiesQueue.Num();

    for (int i = 0; i < n; i++)
    {
        AEnemyCharacterBase* Enemy = EnemiesQueue[i]->EnemyRef;
        EnemiesQueue[i]->EnemyRef = Enemy;

        // If we have tickets to give, grant it to the enemy
        if (MeleeTickets > 0)
        {
            GiveTicket(Enemy, TAG_EnemyAI_Ticket_Melee);
            // Reset their consolation points
            EnemiesQueue[i]->ScoreByMissedTurns = 0.0f;
        }
        else
        {
            // Grant consolation points to increase their likelihood of getting their ticket next loop.
            EnemiesQueue[i]->ScoreByMissedTurns += Enemy->ActorHasTag("Elite") ? ScoreBoostForMissedEliteTurns : ScoreBoostForMissedTurns;
        }
        EnemyScoreboard[Enemy] = EnemiesQueue[i];
    }
}

