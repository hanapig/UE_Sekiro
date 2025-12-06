// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved.

#include "Components/TempestBaseComponent.h"

// Sets default values for this component's properties
UTempestBaseComponent::UTempestBaseComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    SetComponentTickEnabled(false);
    PrimaryComponentTick.bStartWithTickEnabled = false;

    // ...
}

// Called when the game starts
void UTempestBaseComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner() != nullptr)
    {
        if (!GetOwner()->OnDestroyed.IsAlreadyBound(this, &UTempestBaseComponent::OnOwnerDestroyed))
        {
            GetOwner()->OnDestroyed.AddDynamic(this, &UTempestBaseComponent::OnOwnerDestroyed);
        }
    }
}

// Called every frame
void UTempestBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UTempestBaseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (GetOwner() != nullptr)
    {
        GetOwner()->OnDestroyed.RemoveDynamic(this, &UTempestBaseComponent::OnOwnerDestroyed);
    }
}

void UTempestBaseComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
}