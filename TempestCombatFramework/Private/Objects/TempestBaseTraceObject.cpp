// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#include "Objects/TempestBaseTraceObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Components/TempestCollisionManagerComponent.h"
#include "Objects/TempestAttackPropertiesObject.h"
#include "GameFramework/Controller.h"

void UTempestBaseTraceObject::ConstructTrace_Implementation()
{
}

void UTempestBaseTraceObject::ToggleTrace_Implementation(bool bToggle)
{
}

void UTempestBaseTraceObject::OnTraceHit_Implementation(FHitResult HitResult)
{
}

bool UTempestBaseTraceObject::GetCanHitTargetActor_Implementation(const AActor* TargetActor)
{
    return true;
}

bool UTempestBaseTraceObject::GetCanHitTargetComponent_Implementation(const UPrimitiveComponent* TargetComponent)
{
    return true;
}

void UTempestBaseTraceObject::GetCollisionTraceHits_Implementation(TArray<FHitResult>& FoundHitResult)
{
}

class AController* UTempestBaseTraceObject::GetTraceInstigator_Implementation()
{
    if (TraceOwner != nullptr)
    {
        if (UTempestCollisionManagerComponent* const TempestCollisionManagerComponent = TraceOwner->FindComponentByClass<UTempestCollisionManagerComponent>())
        {
            return TempestCollisionManagerComponent->TracesInstigator;
        }
    }

    return nullptr;
}

void UTempestBaseTraceObject::SetTraceMeshInfo(UPrimitiveComponent* NewPrimitiveComponent, TArray<FName> PrimitiveComponentSocketNames)
{
    TracePrimitiveComponent = NewPrimitiveComponent;
    TracePrimitiveComponentSocketNames = PrimitiveComponentSocketNames;
}

void UTempestBaseTraceObject::SetTraceOwner(AActor* InNewTraceOwner)
{
    if (InNewTraceOwner != nullptr)
    {
        TraceOwner = InNewTraceOwner;
    }
}

void UTempestBaseTraceObject::AddTraceCurrentTrajectory(const FName& InBoneName, const FVector& InCurrentTraceTrajectoryLocation)
{
    // Check if the bone exists in the map
    if (!TraceTrajectory.Contains(InBoneName))
    {
        // Initialize a new FArrayOfVectors for this bone if it doesn't exist
        FArrayOfVectors NewArray;
        NewArray.VectorLocation.Emplace(InCurrentTraceTrajectoryLocation);
        TraceTrajectory.Emplace(InBoneName, NewArray);
        return;
    }

    // Add the location to the existing FArrayOfVectors
    TraceTrajectory[InBoneName].VectorLocation.Emplace(InCurrentTraceTrajectoryLocation);
}

void UTempestBaseTraceObject::ConstructAttackPropertyOfObject(UTempestAttackPropertiesObject* AttackPropertyToAssign)
{
    if (AttackPropertyToAssign)
    {
        UTempestAttackPropertiesObject* LocalNewAttackProperty = DuplicateObject(AttackPropertyToAssign, TraceOwner);
        AttackProperty = LocalNewAttackProperty;
        AttackProperty->SetAttackPropertyOwner(TraceOwner);
        AttackProperty->SetAttackPropertyInstigator(GetTraceInstigator());
        AttackProperty->SetHitAttackTrace(this);
        AttackProperty->ConstructAttackProperty();
    }
}

void UTempestBaseTraceObject::DoCollisionTrace_Implementation()
{
}

void UTempestBaseTraceObject::RecordCollisionTrajectory_Implementation()
{
    if (TracePrimitiveComponent == nullptr || TracePrimitiveComponentSocketNames.Num() == 0)
    {
        return;
    }

    for (const FName& SocketName : TracePrimitiveComponentSocketNames)
    {
        if (TracePrimitiveComponent->DoesSocketExist(SocketName))
        {
            FVector SocketCurrentLocation = TracePrimitiveComponent->GetSocketLocation(SocketName);
            AddTraceCurrentTrajectory(SocketName, SocketCurrentLocation);
        }
    }
}

void UTempestBaseTraceObject::ClearTraceInformation_Implementation()
{
    HitActors.Empty();
    HitComponents.Empty();
    TraceTrajectory.Empty();
    CurrentHitResult = FHitResult();
    AttackProperty = nullptr;
}