#include "EngineSystems/EnemyManagerTick.h"
#include "Systems/EnemyGroupBehaviorManager.h"

void FEnemyManagerTick::ExecuteTick(
	float DeltaTime,
	ELevelTick TickType,
	ENamedThreads::Type CurrentThread,
	const FGraphEventRef& MyCompletionGraphEvent)
{
	if (Target && IsValid(Target))
	{
		/*Target->PrePhysicsUpdate(DeltaTime);*/
	}
}
