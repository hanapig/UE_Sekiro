// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#include "Components/TempestCombatComponent.h"
#include "Objects/TempestBaseBuffObject.h"
#include "Objects/TempestAttackPropertiesObject.h"
#include "Objects/TempestDefensePropertiesObject.h"
#include "Objects/TempestBaseConditionObject.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "HAL/PlatformProcess.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UTempestCombatComponent::UTempestCombatComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    // ...
}

// Called when the game starts
void UTempestCombatComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

// Called every frame
void UTempestCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UTempestCombatComponent::ModifyCombatStatus(FGameplayTagContainer CombatStatusToModify, EModifyingType ModifyType)
{
    if (ModifyType == EModifyingType::Add)
    {
        CombatStatusContainer.AppendTags(CombatStatusToModify);

        for (const FGameplayTag& Tag : CombatStatusToModify)
        {
            OnModifiedCombatStatus.Broadcast(Tag, ModifyType);
        }
    }
    else
    {
        CombatStatusContainer.RemoveTags(CombatStatusToModify);
        for (const FGameplayTag& Tag : CombatStatusToModify)
        {
            OnModifiedCombatStatus.Broadcast(Tag, ModifyType);
        }
    }
}

void UTempestCombatComponent::AddToAppliedBuffs(UTempestBaseBuffObject* BuffToAdd)
{
    if (!AppliedBuffs.Contains(BuffToAdd))
    {
        AppliedBuffs.Emplace(BuffToAdd);
        OnOwnerBuffApplied.Broadcast(BuffToAdd);
    }
}

void UTempestCombatComponent::RemoveFromAppliedBuffs(UTempestBaseBuffObject* BuffToRemove)
{
    if (AppliedBuffs.Contains(BuffToRemove))
    {
        AppliedBuffs.Remove(BuffToRemove);
        OnOwnerBuffRemoved.Broadcast(BuffToRemove);
    }
}

void UTempestCombatComponent::SetReceivedAttackProperty(UTempestAttackPropertiesObject* NewAttackProperty)
{
    ReceivedAttackProperty = NewAttackProperty;
    OnUpdatedReceivedAttackProperty.Broadcast();
}

void UTempestCombatComponent::SetAttackProperty(UTempestAttackPropertiesObject* NewAttackProperty)
{
    OwnerAttackProperty = NewAttackProperty;
    OnUpdatedAttackProperty.Broadcast();
}

void UTempestCombatComponent::ConstructDefensePropertyOfClass(TSubclassOf<UTempestDefensePropertiesObject> PropertyToConstruct, UTempestDefensePropertiesObject*& ConstructedProperty)
{
    ConstructedProperty = nullptr;
    if (PropertyToConstruct)
    {
        UTempestDefensePropertiesObject* LocalNewProperty;
        LocalNewProperty = NewObject<UTempestDefensePropertiesObject>(GetOwner(), PropertyToConstruct);

        LocalNewProperty->SetDefensePropertyOwner(GetOwner());
        LocalNewProperty->ConstructDefenseProperty();
        ConstructedProperty = LocalNewProperty;
    }
}

void UTempestCombatComponent::GetCombatTarget(AActor*& FoundCombatTarget)
{
    if (CombatTargets.Num() > 0)
    {
        FoundCombatTarget = CombatTargets[0];
    }
}

void UTempestCombatComponent::GetCombatTargets(TArray<AActor*>& FoundCombatTargets)
{
    FoundCombatTargets = CombatTargets;
}

UTempestBaseBuffObject* UTempestCombatComponent::GetAppliedBuffOfGameplayTag(FGameplayTag InBuffTag)
{
    for (UTempestBaseBuffObject* Elem : AppliedBuffs)
    {
        if (Elem != nullptr && Elem->BuffGameplayTag == InBuffTag)
        {
            return Elem;
        }
    }

    return nullptr;
}

UTempestBaseBuffObject* UTempestCombatComponent::GetAppliedBuffOfClass(TSubclassOf<UTempestBaseBuffObject> InBuffClass)
{
    for (UTempestBaseBuffObject* Elem : AppliedBuffs)
    {
        if (Elem != nullptr && Elem->GetClass() == InBuffClass)
        {
            return Elem;
        }
    }

    return nullptr;
}