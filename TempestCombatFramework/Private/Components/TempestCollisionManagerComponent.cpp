// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#include "Components/TempestCollisionManagerComponent.h"
#include "Components/TempestTickingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Objects/TempestBaseTraceObject.h"

// Sets default values for this component's properties
UTempestCollisionManagerComponent::UTempestCollisionManagerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    // ...
}

// Called when the game starts
void UTempestCollisionManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (ACharacter* const OwningCharacter = Cast<ACharacter>(GetOwner()))
    {
        TracesInstigator = OwningCharacter->GetController();
    }
}

// Called every frame
void UTempestCollisionManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTempestCollisionManagerComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
    ClearAvailableTraceObjects();
}

void UTempestCollisionManagerComponent::ConstructTraceOfClass(TSubclassOf< class UTempestBaseTraceObject> TraceClassToConstruct, UTempestBaseTraceObject*& CreatedTraceObject)
{
    CreatedTraceObject = nullptr;
    if (TraceClassToConstruct)
    {
        UTempestBaseTraceObject* LocalNewTrace;
        LocalNewTrace = NewObject<UTempestBaseTraceObject>(GetOwner(), TraceClassToConstruct);
        CreatedTraceObjects.AddUnique(LocalNewTrace);
        LocalNewTrace->TraceOwner = GetOwner();
        LocalNewTrace->ConstructTrace();
        CreatedTraceObject = LocalNewTrace;
    }
}

void UTempestCollisionManagerComponent::GetTraceOfGameplayTag(FGameplayTag TraceCollisionToFind, UTempestBaseTraceObject*& FoundTraceCollision)
{
    FoundTraceCollision = nullptr;
    if (CreatedTraceObjects.Num() > 0)
    {
        for (int32 i = 0; i < CreatedTraceObjects.Num(); i++)
        {
            if (CreatedTraceObjects[i])
            {
                if (CreatedTraceObjects[i]->TraceGameplayTag == TraceCollisionToFind)
                {
                    FoundTraceCollision = CreatedTraceObjects[i];
                    return;
                }
            }
        }
    }
}

void UTempestCollisionManagerComponent::GetTraceByClass(TSubclassOf< class UTempestBaseTraceObject> TraceToFind, UTempestBaseTraceObject*& FoundTrace)
{
    FoundTrace = nullptr;
    if (TraceToFind)
    {
        if (CreatedTraceObjects.Num() > 0)
        {
            for (int32 i = 0; i < CreatedTraceObjects.Num(); i++)
            {
                if (CreatedTraceObjects[i])
                {
                    if (CreatedTraceObjects[i]->GetClass() == TraceToFind)
                    {
                        FoundTrace = CreatedTraceObjects[i];
                        return;
                    }
                }
            }
        }
    }
}

void UTempestCollisionManagerComponent::RemoveTraceFromActiveTraces(UTempestBaseTraceObject* TraceToRemove)
{
    if (TraceToRemove)
    {
        if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
        {
            TickingComponent->RemoveTickableObject(TraceToRemove);
        }
        ActiveTraceObjects.Remove(TraceToRemove);
    }
}

void UTempestCollisionManagerComponent::RemoveTraceFromCreatedTraces(UTempestBaseTraceObject* TraceToRemove)
{
    if (TraceToRemove)
    {
        if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
        {
            TickingComponent->RemoveTickableObject(TraceToRemove);
        }
        CreatedTraceObjects.Remove(TraceToRemove);
    }
}

void UTempestCollisionManagerComponent::ClearAvailableTraceObjects()
{
    for (int32 i = 0; i < ActiveTraceObjects.Num(); i++)
    {
        if (ActiveTraceObjects[i])
        {
            if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
            {
                TickingComponent->RemoveTickableObject(ActiveTraceObjects[i]);
            }
        }
    }
    for (int32 i = 0; i < CreatedTraceObjects.Num(); i++)
    {
        if (CreatedTraceObjects[i])
        {
            if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
            {
                TickingComponent->RemoveTickableObject(CreatedTraceObjects[i]);
            }
        }
    }

    ActiveTraceObjects.Empty();
    CreatedTraceObjects.Empty();
}

void UTempestCollisionManagerComponent::SetTracesInstigator(class AController* InNewTracesInstigator)
{
    if (InNewTracesInstigator != nullptr)
    {
        TracesInstigator = InNewTracesInstigator;
    }
}