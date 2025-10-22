#include "EnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer &ObjectInitializer)
:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
    
}

void AEnemyAIController::SetPlayerPawn(APawn* PlayerRef)
{
	PlayerPawn = PlayerRef;
	Blackboard->SetValueAsObject("Target",PlayerRef);
	Blackboard->SetValueAsBool("InCombat",true);
	
}
