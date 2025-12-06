// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#include "Components/TempestBaseStateManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TempestTickingComponent.h"

// Sets default values for this component's properties
UTempestBaseStateManagerComponent::UTempestBaseStateManagerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    // ...
}

// Called when the game starts
void UTempestBaseStateManagerComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UTempestBaseStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UTempestBaseStateManagerComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
    ClearStatesComponent();
}

void UTempestBaseStateManagerComponent::PerformStateOfClass(TSubclassOf<UTempestBaseStateObject> StateToSet)
{
    TryPerformStateOfClass(StateToSet, false);
}

bool UTempestBaseStateManagerComponent::TryPerformStateOfClass(TSubclassOf<UTempestBaseStateObject> StateToSet, bool ConditionCheck /*= true*/)
{
    if (StateToSet)
    {
        UTempestBaseStateObject* LocalState = nullptr;
        GetStateOfClass(StateToSet, LocalState);

        if (LocalState)
        {
            if (ConditionCheck)
            {
                if (LocalState->CanPerformState())
                {
                    LocalState->PreStateActivation();
                    LocalState->StartState();
                    LocalState->PostStateActivation();
                    return true;
                }
            }
            else
            {
                LocalState->PreStateActivation();
                LocalState->StartState();
                LocalState->PostStateActivation();
                return true;
            }

            return false;
        }
        else
        {
            ConstructStateOfClass(StateToSet, LocalState);

            if (ConditionCheck)
            {
                if (LocalState->CanPerformState())
                {
                    LocalState->PreStateActivation();
                    LocalState->StartState();
                    LocalState->PostStateActivation();
                    return true;
                }
            }
            else
            {
                LocalState->PreStateActivation();
                LocalState->StartState();
                LocalState->PostStateActivation();
                return true;
            }
        }
    }
    return false;
}

bool UTempestBaseStateManagerComponent::TryPerformStatesOfClass(TArray <TSubclassOf<UTempestBaseStateObject>> StatesToSet, bool ConditionCheck /*= true*/)
{
    bool LocalBool = false;
    for (int32 i = 0; i < StatesToSet.Num(); i++)
    {
        if (StatesToSet[i])
        {
            LocalBool = TryPerformStateOfClass(StatesToSet[i], ConditionCheck);
            if (LocalBool)
            {
                return true;
            }
        }
    }
    return false;
}

void UTempestBaseStateManagerComponent::RemoveFromActivatableStates(UTempestBaseStateObject* StateToRemove)
{
    if (!StateToRemove)
    {
        return;
    }

    if (ActivatableStates.Contains(StateToRemove))
    {
        if (CurrentActiveState)
        {
            if (CurrentActiveState == StateToRemove)
            {
                SetCurrentActiveState(nullptr);
            }
        }
        ActivatableStates.Remove(StateToRemove);
        if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
        {
            TickingComponent->RemoveTickableObject(StateToRemove);
        }
        StateToRemove->OnRemovedState();
    }
}

void UTempestBaseStateManagerComponent::TrySetQueuedState(TSubclassOf<UTempestBaseStateObject> StateToQueue, bool ConditionCheck /*= true*/)
{
    if (StateToQueue)
    {
        if (ConditionCheck)
        {
            if (GetCanSetQueuedStateOfClass(StateToQueue))
            {
                AddToQueuedStates(StateToQueue);
            }
        }
        else
        {
            AddToQueuedStates(StateToQueue);
        }
    }
}

void UTempestBaseStateManagerComponent::SetCurrentActiveState(UTempestBaseStateObject* NewCurrentActiveState)
{
    UTempestBaseStateObject* LocalCurrentState = nullptr;
    bool identicalValue = false;
    if (NewCurrentActiveState == CurrentActiveState)
    {
        identicalValue = true;
    }

    if (CurrentActiveState)
    {
        if (!identicalValue)
        {
            LocalCurrentState = CurrentActiveState;
            CurrentActiveState->PreLossOfActiveState();
        }
    }
    CurrentActiveState = NewCurrentActiveState;
    OnUpdatedCurrentActiveState.Broadcast();

    if (LocalCurrentState && !identicalValue)
    {
        LocalCurrentState->PostLossOfActiveState();
    }

    if (NewCurrentActiveState)
    {
        if (PassiveStates.Num() > 0)
        {
            for (int32 i = 0; i < PassiveStates.Num(); i++)
            {
                if (PassiveStates[i] == NewCurrentActiveState)
                {
                    RemoveFromPassiveStates(NewCurrentActiveState);
                }
            }
        }
    }
}

void UTempestBaseStateManagerComponent::SetAsPassiveState(UTempestBaseStateObject* NewPassiveState)
{
    if (NewPassiveState)
    {
        PassiveStates.AddUnique(NewPassiveState);
        if (NewPassiveState == CurrentActiveState)
        {
            CurrentActiveState = nullptr;
        }
    }
}

void UTempestBaseStateManagerComponent::GetStateOfClass(TSubclassOf<UTempestBaseStateObject> StateToSearch, UTempestBaseStateObject*& FoundState)
{
    for (int32 i = 0; i < ActivatableStates.Num(); i++)
    {
        if (ActivatableStates[i])
        {
            if (ActivatableStates[i]->GetClass() == StateToSearch)
            {
                FoundState = ActivatableStates[i];

                return;
            }
        }
    }

    FoundState = nullptr;
    return;
}

void UTempestBaseStateManagerComponent::GetChildStateOfClass(TSubclassOf<UTempestBaseStateObject> StateToSearch, UTempestBaseStateObject*& FoundState)
{
    for (int32 i = 0; i < ActivatableStates.Num(); i++)
    {
        if (ActivatableStates[i])
        {
            if (UKismetMathLibrary::ClassIsChildOf(ActivatableStates[i]->GetClass(), StateToSearch))
            {
                FoundState = ActivatableStates[i];

                return;
            }
        }
    }

    FoundState = nullptr;
    return;
}

bool UTempestBaseStateManagerComponent::GetCanPerformStateOfClass(TSubclassOf<UTempestBaseStateObject> ClassToLookFor)
{
    if (ClassToLookFor)
    {
        UTempestBaseStateObject* LocalState = nullptr;
        GetStateOfClass(ClassToLookFor, LocalState);

        if (LocalState)
        {
            return LocalState->CanPerformState();
        }
        else
        {
            UTempestBaseStateObject* LocalNewState;
            ConstructStateOfClass(ClassToLookFor, LocalNewState);

            return LocalNewState->CanPerformState();
        }
    }
    return false;
}

bool UTempestBaseStateManagerComponent::GetCanSetQueuedStateOfClass(TSubclassOf<UTempestBaseStateObject> StateToSearch)
{
    UTempestBaseStateObject* LocalState = nullptr;
    GetStateOfClass(StateToSearch, LocalState);

    if (LocalState)
    {
        return LocalState->CanSetAsQueueState();
    }
    else
    {
        UTempestBaseStateObject* LocalNewState;
        ConstructStateOfClass(StateToSearch, LocalNewState);

        return LocalNewState->CanSetAsQueueState();
    }
}

void UTempestBaseStateManagerComponent::RemoveFromPassiveStates(UTempestBaseStateObject* StateToRemove)
{
    if (StateToRemove)
    {
        if (PassiveStates.Contains(StateToRemove))
        {
            PassiveStates.Remove(StateToRemove);
            if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
            {
                TickingComponent->RemoveTickableObject(StateToRemove);
            }
            StateToRemove->OnRemovedState();
        }
    }
}

UTempestBaseStateObject*& UTempestBaseStateManagerComponent::GetStateOfGameplayTag(FGameplayTag StateGameplayTag)
{
    for (int32 i = 0; i < ActivatableStates.Num(); i++)
    {
        if (ActivatableStates[i])
        {
            if (ActivatableStates[i]->StateGameplayTag == StateGameplayTag)
            {
                return ActivatableStates[i];
            }
        }
    }
    static UTempestBaseStateObject* LocalState = nullptr;
    return LocalState;
}

void UTempestBaseStateManagerComponent::ConstructStateOfClass(TSubclassOf<UTempestBaseStateObject> StateToConstruct, UTempestBaseStateObject*& ConstructedState)
{
    ConstructedState = nullptr;
    if (StateToConstruct)
    {
        UTempestBaseStateObject* LocalNewState;
        LocalNewState = NewObject<UTempestBaseStateObject>(GetOwner(), StateToConstruct);

        ActivatableStates.AddUnique(LocalNewState);
        LocalNewState->SetPerformingActor(GetOwner());

        LocalNewState->ConstructState();
        ConstructedState = LocalNewState;
    }
}

void UTempestBaseStateManagerComponent::ClearStatesComponent()
{
    TArray<UTempestBaseStateObject*> LocalArray;
    LocalArray.Append(ActivatableStates);
    //LocalArray.Append(QueuedStates);
    LocalArray.Append(PassiveStates);
    for (int32 i = 0; i < LocalArray.Num(); i++)
    {
        if (LocalArray[i])
        {
            if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
            {
                TickingComponent->RemoveTickableObject(LocalArray[i]);
            }
            LocalArray[i]->OnRemovedState();
        }
    }
    ActivatableStates.Empty();
    QueuedStates.Empty();
    PassiveStates.Empty();

    CurrentActiveState = nullptr;
}