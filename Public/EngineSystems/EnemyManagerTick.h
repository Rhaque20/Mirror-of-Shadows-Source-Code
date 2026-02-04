#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"

struct FEnemyManagerTick : public FTickFunction
{
    class AEnemyGroupBehaviorManager* Target = nullptr;

    virtual void ExecuteTick(
        float DeltaTime,
        ELevelTick TickType,
        ENamedThreads::Type CurrentThread,
        const FGraphEventRef& MyCompletionGraphEvent) override;

    virtual FString DiagnosticMessage() override
    {
        return TEXT("FEnemyManagerDuringPhysicsTick");
    }
};

// IMPORTANT: Traits must be in the header
template<>
struct TStructOpsTypeTraits<FEnemyManagerTick>
    : public TStructOpsTypeTraitsBase2<FEnemyManagerTick>
{
    enum { WithCopy = false };
};