// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TempestTickingComponent.h"
#include "Interfaces/TempestTickingInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UTempestTickingComponent::UTempestTickingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTempestTickingComponent::AddTickableObject(UObject* Object)
{
    if (Object != nullptr)
    {
        if (!(TickableObjects.Contains(Object)))
        {
            bool Interface = UKismetSystemLibrary::DoesImplementInterface(Object, UTempestTickingInterface::StaticClass());
            if (Interface)
            {
                TickableObjects.Emplace(Object);
            }
        }
    }
}

void UTempestTickingComponent::RemoveTickableObject(UObject* Object)
{
    if (Object != nullptr)
    {
        TickableObjects.Remove(Object);
    }
}

void UTempestTickingComponent::GetTickableObjects(TArray <UObject*>& FoundTickableObjects)
{
    FoundTickableObjects = TickableObjects;
}

void UTempestTickingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (int32 i = 0; i < TickableObjects.Num(); i++)
    {
        if (TickableObjects[i])
        {
            ITempestTickingInterface::Execute_TickTempestObject(TickableObjects[i], DeltaTime);
        }
    }
}