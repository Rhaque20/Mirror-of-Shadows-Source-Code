// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterGrowthComponent.h"

#include "AbilitySystemInterface.h"
#include "SavedGame.h"
#include "PlayerCharacters.h"
#include "CustomGameInstance.h"

// Sets default values for this component's properties
UCharacterGrowthComponent::UCharacterGrowthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    bWantsInitializeComponent = true;
    PhaseNumber = 2;

	// ...
}
void UCharacterGrowthComponent::InitializeComponent()
{
    Super::InitializeComponent();

    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance && GameInstance->SaveSystem)
    {
        TScriptInterface<ISavableInterface> Interface(this);
        GameInstance->SaveSystem->RegisterSubsystem(Interface);
    }
}

// Called when the game starts
void UCharacterGrowthComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerOwner = Cast<APlayerCharacters>(GetOwner());
	if (PlayerOwner)
	{
		AbilitySystem = PlayerOwner->GetAbilitySystemComponent();
		ReceiveXP(0.0f);
	}
	// ...
	
}

void UCharacterGrowthComponent::ReceiveXP(float BaseXPAmount)
{
	float FinalXPReward = FMath::Floor(BaseXPAmount);
	TotalXP += FinalXPReward;
	TotalAP += FinalXPReward;

	if (TotalXP >= BaseTotalXPRow.Eval(CharacterLevel+1,FString("TotalEXP")))
	{
		CharacterLevel++;
		UE_LOG(LogTemp,Display,TEXT("LEVEL UP!"));
		PlayerOwner->InitializeAttributes(CharacterLevel);
		OnLevelUp.Broadcast(PlayerOwner,CharacterLevel);
	}

	float XPDifference = TotalXP - BaseTotalXPRow.Eval(CharacterLevel,FString("XPDifference"));
	OnXPChange.Broadcast(PlayerOwner,XPDifference,XPRequirementRow.Eval(CharacterLevel,FString("XPDifference")));
	OnAPChange.Broadcast(TotalAP,TotalXP);
}

void UCharacterGrowthComponent::DeductAP(float APCost)
{
	if (TotalAP >= APCost)
	{
		TotalAP -= APCost;
		OnAPChange.Broadcast(TotalAP,TotalXP);
	}
}

void UCharacterGrowthComponent::ReceiveSkillTreeNode(USkillTreeNodeData* SkillTreeNode, FName SkillID)
{
	ActiveSkillTreeNodes.Add(SkillTreeNode);
	ActiveSkillTreeNodeNames.Add(SkillID);
}

int32 UCharacterGrowthComponent::GetSavePhase() const
{
    return PhaseNumber;
}

void UCharacterGrowthComponent::SaveData(USavedGame& Root)
{
	FSkillTreeSaveData SaveData = FSkillTreeSaveData();
    for (USkillTreeNodeData* Node : ActiveSkillTreeNodes)
    {
        if (Node)
        {
            SaveData.ActiveSkillTreeNodes.Add(TSoftObjectPtr<USkillTreeNodeData>(Node));
        }
    }
    SaveData.ActiveSkillTreeNodeNames = ActiveSkillTreeNodeNames;
	SaveData.CurrentAP = TotalAP;
	SaveData.CurrentXP = TotalXP;
	SaveData.Level = CharacterLevel;
	
	Root.SkillTreeData.Add(PlayerOwner->GetIdentifyingTag(),SaveData);
}

void UCharacterGrowthComponent::LoadData(const USavedGame& Root)
{
    ActiveSkillTreeNodes.Empty();
    ActiveSkillTreeNodeNames.Empty();
	
	PlayerOwner = Cast<APlayerCharacters>(GetOwner());
	
	const FSkillTreeSaveData* SaveData = Root.SkillTreeData.Find(PlayerOwner->GetIdentifyingTag());
	
	if (!SaveData)
		return;

    for (const TSoftObjectPtr<USkillTreeNodeData>& SoftNode :
        SaveData->ActiveSkillTreeNodes)
    {
        if (USkillTreeNodeData* LoadedNode = SoftNode.LoadSynchronous())
        {
            ActiveSkillTreeNodes.Add(LoadedNode);
        	PlayerOwner->ActivateSkillNode(LoadedNode);
        }
    }

    ActiveSkillTreeNodeNames = SaveData->ActiveSkillTreeNodeNames;
	TotalXP = SaveData->CurrentXP;
	TotalAP = SaveData->CurrentAP;
	CharacterLevel = SaveData->Level;
}

void UCharacterGrowthComponent::InitializeData()
{
	ISavableInterface::InitializeData();	
}

