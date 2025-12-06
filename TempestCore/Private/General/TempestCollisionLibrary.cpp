// Fill out your copyright notice in the Description page of Project Settings.

#include "General/TempestCollisionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/BoxElem.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/SphereElem.h"
#include "PhysicsEngine/SphylElem.h"
#include "PhysicsEngine/SkeletalBodySetup.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

UTempestCollisionLibrary::UTempestCollisionLibrary()
{
}

void UTempestCollisionLibrary::UpdateCollisionResponseToChannelForBones(USkeletalMeshComponent* InSkeletalMesh, const TArray<FName>& InBoneNames, ECollisionChannel InChannel, ECollisionResponse InNewResponse)
{
    if (InSkeletalMesh != nullptr && !InBoneNames.IsEmpty())
    {
        if (UPhysicsAsset* const PhysicsAsset = InSkeletalMesh->GetPhysicsAsset())
        {
            // Loop through all the bodies in the physics asset
            for (UBodySetup* BodySetup : PhysicsAsset->SkeletalBodySetups)
            {
                if (BodySetup && InBoneNames.Contains(BodySetup->BoneName)) // or other naming convention
                {
                    // Get the BodyInstance for the current body setup
                    if (FBodyInstance* const BodyInstance = InSkeletalMesh->GetBodyInstance(BodySetup->BoneName))
                    {
                        // Loop through all shapes and toggle collision
                        // Get all shapes for the body instance
                        TArray<FPhysicsShapeHandle> Shapes;
                        BodyInstance->GetAllShapes_AssumesLocked(Shapes);

                        for (int32 ShapeIndex = 0; ShapeIndex < Shapes.Num(); ++ShapeIndex)
                        {
                            /* Set the shape collision enabled state
                            BodyInstance->SetShapeCollisionEnabled(ShapeIndex, InCollisionType, false);*/

                            // Create a collision response container for this shape
                            FCollisionResponseContainer NewResponseContainer;
                            NewResponseContainer.SetResponse(InChannel, InNewResponse); // Set to ignore the specified channel
                            // Apply the response to the shape
                            BodyInstance->SetShapeResponseToChannels(ShapeIndex, NewResponseContainer);
                        }

                        BodyInstance->UpdatePhysicsFilterData();
                    }
                }
            }
        }
    }
}

void UTempestCollisionLibrary::TraceWithPhysicsAssetBoneShapes(class UPrimitiveComponent* InMesh, const TArray<FName>& InBoneNames, ECollisionChannel TraceChannel, const TArray<AActor*>& InActorsToIgnore, TArray<FHitResult>& OutHitResults, bool InShowDebug, float ScaleMultiplier /*= 1.0f*/)
{
    if (!InMesh || InBoneNames.IsEmpty())
    {
        return;
    }

    USkeletalMeshComponent* const SkeletalMesh = Cast<USkeletalMeshComponent>(InMesh);
    if (SkeletalMesh == nullptr)
    {
        return;
    }

    UPhysicsAsset* const PhysicsAsset = SkeletalMesh->GetPhysicsAsset();
    if (!PhysicsAsset)
    {
        return;
    }

    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = false;
    QueryParams.AddIgnoredActors(InActorsToIgnore);

    FVector ComponentScale = SkeletalMesh->GetComponentScale();
    TArray<FOverlapResult> OutOverlaps;

    FCollisionResponseParams ResponseParams;

    for (TObjectPtr<USkeletalBodySetup>& BodySetup : PhysicsAsset->SkeletalBodySetups)
    {
        if (BodySetup && InBoneNames.Contains(BodySetup->BoneName))
        {
            if (FBodyInstance* const BodyInstance = SkeletalMesh->GetBodyInstance(BodySetup->BoneName))
            {
                FTransform BoneTransform = BodyInstance->GetUnrealWorldTransform(true);

                // Capsule elements
                for (const FKSphylElem& CapsuleElem : BodySetup->AggGeom.SphylElems)
                {
                    FVector ScaledCenter = CapsuleElem.Center * ComponentScale;
                    FTransform LocalCapsuleTransform(CapsuleElem.Rotation.Quaternion(), ScaledCenter);
                    FTransform ShapeTransform = LocalCapsuleTransform * BoneTransform;

                    float ScaledRadius = CapsuleElem.GetScaledRadius(ComponentScale) * ScaleMultiplier;
                    float ScaledHalfLength = CapsuleElem.GetScaledHalfLength(ComponentScale) * ScaleMultiplier;
                    FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(ScaledRadius, ScaledHalfLength);

                    SkeletalMesh->GetWorld()->OverlapMultiByChannel(
                        OutOverlaps,
                        ShapeTransform.GetLocation(),
                        ShapeTransform.GetRotation(),
                        TraceChannel,
                        CollisionShape,
                        QueryParams,
                        ResponseParams
                    );

                    if (InShowDebug)
                    {
                        DrawDebugCapsule(
                            SkeletalMesh->GetWorld(),
                            ShapeTransform.GetLocation(),
                            ScaledHalfLength,
                            ScaledRadius,
                            ShapeTransform.GetRotation(),
                            FColor::Red,
                            false,
                            0.1f
                        );
                    }
                }

                // Box elements
                for (const FKBoxElem& BoxElem : BodySetup->AggGeom.BoxElems)
                {
                    FVector ScaledCenter = BoxElem.Center * ComponentScale;
                    FTransform LocalBoxTransform(BoxElem.Rotation.Quaternion(), ScaledCenter);
                    FTransform ShapeTransform = LocalBoxTransform * BoneTransform;

                    FVector ScaledExtent(BoxElem.X * 0.5f * ComponentScale.X * ScaleMultiplier,
                        BoxElem.Y * 0.5f * ComponentScale.Y * ScaleMultiplier,
                        BoxElem.Z * 0.5f * ComponentScale.Z * ScaleMultiplier);
                    FCollisionShape CollisionShape = FCollisionShape::MakeBox(ScaledExtent);

                    SkeletalMesh->GetWorld()->OverlapMultiByChannel(
                        OutOverlaps,
                        ShapeTransform.GetLocation(),
                        ShapeTransform.GetRotation(),
                        TraceChannel,
                        CollisionShape,
                        QueryParams,
                        ResponseParams
                    );

                    if (InShowDebug)
                    {
                        DrawDebugBox(
                            SkeletalMesh->GetWorld(),
                            ShapeTransform.GetLocation(),
                            ScaledExtent,
                            ShapeTransform.GetRotation(),
                            FColor::Green,
                            false,
                            0.1f
                        );
                    }
                }

                // Sphere elements
                for (const FKSphereElem& SphereElem : BodySetup->AggGeom.SphereElems)
                {
                    FVector ScaledCenter = SphereElem.Center * ComponentScale * ScaleMultiplier;
                    FTransform LocalSphereTransform(FQuat::Identity, ScaledCenter);
                    FTransform ShapeTransform = LocalSphereTransform * BoneTransform;

                    float ScaledRadius = SphereElem.Radius * ComponentScale.GetMax() * ScaleMultiplier;
                    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ScaledRadius);

                    SkeletalMesh->GetWorld()->OverlapMultiByChannel(
                        OutOverlaps,
                        ShapeTransform.GetLocation(),
                        ShapeTransform.GetRotation(),
                        TraceChannel,
                        CollisionShape,
                        QueryParams,
                        ResponseParams
                    );

                    if (InShowDebug)
                    {
                        DrawDebugSphere(
                            SkeletalMesh->GetWorld(),
                            ShapeTransform.GetLocation(),
                            ScaledRadius,
                            12,
                            FColor::Blue,
                            false,
                            0.1f
                        );
                    }
                }
            }
        }
    }

    // Convert Overlaps to Hit Results for Blueprint compatibility
    for (const FOverlapResult& OverlapResult : OutOverlaps)
    {
        if (OverlapResult.GetActor())
        {
            FHitResult HitResult;
            HitResult.HitObjectHandle = FActorInstanceHandle(OverlapResult.GetActor());
            HitResult.Component = OverlapResult.GetComponent();
            HitResult.Location = OverlapResult.GetActor()->GetActorLocation();
            HitResult.ImpactPoint = HitResult.Location;
            HitResult.ImpactNormal = FVector::UpVector;  // Approximation
            OutHitResults.Emplace(HitResult);
        }
    }
}

TArray<UPrimitiveComponent*> UTempestCollisionLibrary::AddCollisionComponentsFromPhysicsAsset(USkeletalMeshComponent* SkeletalMesh)
{
    TArray<UPrimitiveComponent*> CreatedComponents;
    if (!SkeletalMesh)
    {
        return CreatedComponents;
    }

    // Retrieve Physics Asset
    UPhysicsAsset* const PhysicsAsset = SkeletalMesh->GetPhysicsAsset();
    if (!PhysicsAsset)
    {
        return CreatedComponents;
    }

    const FVector ComponentScale = SkeletalMesh->GetComponentScale();

    // Iterate through each BodySetup in the Physics Asset
    for (TObjectPtr<USkeletalBodySetup>& BodySetup : PhysicsAsset->SkeletalBodySetups)
    {
        if (BodySetup)
        {
            const FTransform BoneTransform = SkeletalMesh->GetSocketTransform(BodySetup->BoneName);

            // Iterate over the geometry elements for each BodySetup
            for (const FKSphylElem& CapsuleElem : BodySetup->AggGeom.SphylElems)
            {
                // Create the capsule component
                UCapsuleComponent* CapsuleComponent = NewObject<UCapsuleComponent>(SkeletalMesh);
                CapsuleComponent->RegisterComponent();

                // Calculate the scaled center and local transform
                FVector ScaledCenter = CapsuleElem.Center * ComponentScale;
                FTransform LocalCapsuleTransform(CapsuleElem.Rotation.Quaternion(), ScaledCenter);

                // Combine with the bone's transform
                FTransform CapsuleTransform = LocalCapsuleTransform * BoneTransform;

                // Apply the capsule's dimensions
                CapsuleComponent->SetCapsuleRadius(CapsuleElem.GetScaledRadius(ComponentScale));
                CapsuleComponent->SetCapsuleHalfHeight(CapsuleElem.GetScaledHalfLength(ComponentScale));

                // Set the final transform and attach to the skeletal mesh
                FTransform RelativeCapsuleTransform = CapsuleTransform.GetRelativeTransform(SkeletalMesh->GetSocketTransform(BodySetup->BoneName));
                CapsuleComponent->SetRelativeTransform(RelativeCapsuleTransform);

                FAttachmentTransformRules AttachmentRules(FAttachmentTransformRules::KeepRelativeTransform);
                CapsuleComponent->AttachToComponent(SkeletalMesh, AttachmentRules, BodySetup->BoneName);
                CapsuleComponent->ComponentTags.Add(BodySetup->BoneName);

                // Add to the created components array
                CreatedComponents.Emplace(CapsuleComponent);
            }

            // Handle Box Elements
            for (const FKBoxElem& BoxElem : BodySetup->AggGeom.BoxElems)
            {
                UBoxComponent* BoxComponent = NewObject<UBoxComponent>(SkeletalMesh);
                BoxComponent->RegisterComponent();

                FVector ScaledCenter = BoxElem.Center * ComponentScale;
                FTransform LocalBoxTransform(BoxElem.Rotation.Quaternion(), ScaledCenter);
                FTransform BoxTransform = LocalBoxTransform * BoneTransform;

                FVector ScaledExtent = FVector(BoxElem.X, BoxElem.Y, BoxElem.Z) * 0.5f * ComponentScale;
                BoxComponent->SetBoxExtent(ScaledExtent);

                FTransform RelativeBoxTransform = BoxTransform.GetRelativeTransform(SkeletalMesh->GetSocketTransform(BodySetup->BoneName));
                BoxComponent->SetRelativeTransform(RelativeBoxTransform);

                FAttachmentTransformRules AttachmentRules(FAttachmentTransformRules::KeepRelativeTransform);
                BoxComponent->AttachToComponent(SkeletalMesh, AttachmentRules, BodySetup->BoneName);
                BoxComponent->ComponentTags.Add(BodySetup->BoneName);

                CreatedComponents.Emplace(BoxComponent);
            }

            // Handle Sphere Elements
            for (const FKSphereElem& SphereElem : BodySetup->AggGeom.SphereElems)
            {
                USphereComponent* SphereComponent = NewObject<USphereComponent>(SkeletalMesh);
                SphereComponent->RegisterComponent();

                FVector ScaledCenter = SphereElem.Center * ComponentScale;
                FTransform LocalSphereTransform(FQuat::Identity, ScaledCenter);
                FTransform SphereTransform = LocalSphereTransform * BoneTransform;

                float ScaledRadius = SphereElem.Radius * ComponentScale.GetMax();
                SphereComponent->SetSphereRadius(ScaledRadius);

                FTransform RelativeSphereTransform = SphereTransform.GetRelativeTransform(SkeletalMesh->GetSocketTransform(BodySetup->BoneName));
                SphereComponent->SetRelativeTransform(RelativeSphereTransform);

                FAttachmentTransformRules AttachmentRules(FAttachmentTransformRules::KeepRelativeTransform);
                SphereComponent->AttachToComponent(SkeletalMesh, AttachmentRules, BodySetup->BoneName);
                SphereComponent->ComponentTags.Add(BodySetup->BoneName);

                CreatedComponents.Emplace(SphereComponent);
            }
        }
    }

    return CreatedComponents;
}