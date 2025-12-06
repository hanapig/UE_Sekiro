// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#include "Components/TempestBaseAbilityManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TempestTickingComponent.h"

// Sets default values for this component's properties
UTempestBaseAbilityManagerComponent::UTempestBaseAbilityManagerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    // ...
}

// Called when the game starts
void UTempestBaseAbilityManagerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTempestBaseAbilityManagerComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
    ClearAbilityComponent();
}

// Called every frame
void UTempestBaseAbilityManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UTempestBaseAbilityManagerComponent::InitializeAbilitySystemComponent_Implementation()
{
}

void UTempestBaseAbilityManagerComponent::PerformAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSet)
{
    TryPerformAbilityOfClass(AbilityToSet, false);
}

bool UTempestBaseAbilityManagerComponent::TryPerformAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSet, bool ConditionCheck /*= true*/)
{
    if (AbilityToSet)
    {
        UTempestBaseAbilityObject* LocalAbility = nullptr;
        GetAbilityOfClass(AbilityToSet, LocalAbility);

        if (LocalAbility)
        {
            if (ConditionCheck)
            {
                if (LocalAbility->CanPerformAbility())
                {
                    LocalAbility->PreAbilityActivation();
                    LocalAbility->StartAbility();
                    LocalAbility->PostAbilityActivation();
                    return true;
                }
            }
            else
            {
                LocalAbility->PreAbilityActivation();
                LocalAbility->StartAbility();
                LocalAbility->PostAbilityActivation();
                return true;
            }

            return false;
        }
        else
        {
            ConstructAbilityOfClass(AbilityToSet, LocalAbility);

            if (ConditionCheck)
            {
                if (LocalAbility->CanPerformAbility())
                {
                    LocalAbility->PreAbilityActivation();
                    LocalAbility->StartAbility();
                    LocalAbility->PostAbilityActivation();

                    return true;
                }
            }
            else
            {
                LocalAbility->PreAbilityActivation();
                LocalAbility->StartAbility();
                LocalAbility->PostAbilityActivation();

                return true;
            }
        }
    }
    return false;
}

bool UTempestBaseAbilityManagerComponent::TryPerformAbilitiesOfClass(TArray <TSubclassOf<UTempestBaseAbilityObject>> AbilitiesToSet, bool ConditionCheck /*= true*/)
{
    bool LocalBool = false;
    for (int32 i = 0; i < AbilitiesToSet.Num(); i++)
    {
        if (AbilitiesToSet[i])
        {
            LocalBool = TryPerformAbilityOfClass(AbilitiesToSet[i], ConditionCheck);
            if (LocalBool)
            {
                return true;
            }
        }
    }
    return false;
}

void UTempestBaseAbilityManagerComponent::SetCurrentActiveAbility(UTempestBaseAbilityObject* NewCurrentActiveAbility)
{
    UTempestBaseAbilityObject* LocalCurrentActiveAbility = nullptr;
    bool identicalValue = false;
    if (NewCurrentActiveAbility == CurrentActiveAbility)
    {
        identicalValue = true;
    }

    if (CurrentActiveAbility)
    {
        if (!identicalValue)
        {
            LocalCurrentActiveAbility = CurrentActiveAbility;
            CurrentActiveAbility->PreLossOfActiveAbility();
        }
    }

    CurrentActiveAbility = NewCurrentActiveAbility;
    OnUpdatedCurrentActiveAbility.Broadcast();

    if (LocalCurrentActiveAbility && !identicalValue)
    {
        LocalCurrentActiveAbility->PostLossOfActiveAbility();
    }

    if (NewCurrentActiveAbility)
    {
        if (PassiveAbilities.Num() > 0)
        {
            for (int32 i = 0; i < PassiveAbilities.Num(); i++)
            {
                if (PassiveAbilities[i] == NewCurrentActiveAbility)
                {
                    RemoveFromPassiveAbilities(NewCurrentActiveAbility);
                }
            }
        }
    }
}

void UTempestBaseAbilityManagerComponent::SetAsPassiveAbility(UTempestBaseAbilityObject* NewPassiveAbility)
{
    if (NewPassiveAbility)
    {
        PassiveAbilities.AddUnique(NewPassiveAbility);
        if (NewPassiveAbility == CurrentActiveAbility)
        {
            CurrentActiveAbility = nullptr;
        }
    }
}

void UTempestBaseAbilityManagerComponent::RemoveFromPassiveAbilities(UTempestBaseAbilityObject* AbilityToRemove)
{
    if (AbilityToRemove)
    {
        if (PassiveAbilities.Contains(AbilityToRemove))
        {
            PassiveAbilities.Remove(AbilityToRemove);

            if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
            {
                TickingComponent->RemoveTickableObject(AbilityToRemove);
            }

            AbilityToRemove->OnRemovedAbility();
        }
    }
}

void UTempestBaseAbilityManagerComponent::RemoveFromActivatableAbilities(UTempestBaseAbilityObject* AbilityToRemove)
{
    if (!AbilityToRemove)
    {
        return;
    }

    if (ActivatableAbilities.Contains(AbilityToRemove))
    {
        if (CurrentActiveAbility)
        {
            if (CurrentActiveAbility == AbilityToRemove)
            {
                SetCurrentActiveAbility(nullptr);
            }
        }
        ActivatableAbilities.Remove(AbilityToRemove);
        if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
        {
            TickingComponent->RemoveTickableObject(AbilityToRemove);
        }
        AbilityToRemove->OnRemovedAbility();
    }
}

UTempestBaseAbilityObject*& UTempestBaseAbilityManagerComponent::GetCurrentActiveAbility()
{
    return CurrentActiveAbility;
}

void UTempestBaseAbilityManagerComponent::GetAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSearch, UTempestBaseAbilityObject*& FoundAbility)
{
    for (int32 i = 0; i < ActivatableAbilities.Num(); i++)
    {
        if (ActivatableAbilities[i])
        {
            if (ActivatableAbilities[i]->GetClass() == AbilityToSearch)
            {
                FoundAbility = ActivatableAbilities[i];

                return;
            }
        }
    }

    FoundAbility = nullptr;
    return;
}

bool UTempestBaseAbilityManagerComponent::GetCanPerformAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSearch)
{
    if (AbilityToSearch)
    {
        UTempestBaseAbilityObject* LocalAbility = nullptr;
        GetAbilityOfClass(AbilityToSearch, LocalAbility);

        if (LocalAbility)
        {
            return LocalAbility->CanPerformAbility();
        }
        else
        {
            UTempestBaseAbilityObject* LocalNewAbility;
            ConstructAbilityOfClass(AbilityToSearch, LocalNewAbility);

            return LocalNewAbility->CanPerformAbility();
        }
    }
    return false;
}

void UTempestBaseAbilityManagerComponent::GetChildAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSearch, UTempestBaseAbilityObject*& FoundAbility)
{
    if (AbilityToSearch)
    {
        for (int32 i = 0; i < ActivatableAbilities.Num(); i++)
        {
            if (ActivatableAbilities[i])
            {
                if (UKismetMathLibrary::ClassIsChildOf(ActivatableAbilities[i]->GetClass(), AbilityToSearch))
                {
                    FoundAbility = ActivatableAbilities[i];

                    return;
                }
            }
        }

        FoundAbility = nullptr;
        return;
    }
}

UTempestBaseAbilityObject*& UTempestBaseAbilityManagerComponent::GetAbilityOfGameplayTag(FGameplayTag AbilityGameplayTag)
{
    for (int32 i = 0; i < ActivatableAbilities.Num(); i++)
    {
        if (ActivatableAbilities[i])
        {
            if (ActivatableAbilities[i]->AbilityGameplayTag == AbilityGameplayTag)
            {
                return ActivatableAbilities[i];
            }
        }
    }
    static UTempestBaseAbilityObject* LocalAbility = nullptr;
    return LocalAbility;
}

void UTempestBaseAbilityManagerComponent::ConstructAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToConstruct, UTempestBaseAbilityObject*& ConstructedAbility)
{
    ConstructedAbility = nullptr;
    if (AbilityToConstruct)
    {
        UTempestBaseAbilityObject* LocalNewAbility;
        LocalNewAbility = NewObject<UTempestBaseAbilityObject>(GetOwner(), AbilityToConstruct);

        ActivatableAbilities.AddUnique(LocalNewAbility);
        LocalNewAbility->SetPerformingActor(GetOwner());
        LocalNewAbility->ConstructAbility();
        ConstructedAbility = LocalNewAbility;
    }
}

void UTempestBaseAbilityManagerComponent::ClearAbilityComponent()
{
    TArray<UTempestBaseAbilityObject*> LocalArray;
    LocalArray.Append(PassiveAbilities);
    LocalArray.Append(ActivatableAbilities);

    for (int32 i = 0; i < LocalArray.Num(); i++)
    {
        if (LocalArray[i])
        {
            if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
            {
                TickingComponent->RemoveTickableObject(LocalArray[i]);
            }
            LocalArray[i]->OnRemovedAbility();
        }
    }

    PassiveAbilities.Empty();
    ActivatableAbilities.Empty();
    CurrentActiveAbility = nullptr;
}