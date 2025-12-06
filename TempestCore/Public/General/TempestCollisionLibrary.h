// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/EngineTypes.h"
#include "TempestCollisionLibrary.generated.h"

/**
 *
 */
UCLASS()
class TEMPESTCORE_API UTempestCollisionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UTempestCollisionLibrary();

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    static void UpdateCollisionResponseToChannelForBones(class USkeletalMeshComponent* InSkeletalMesh, const TArray<FName>& InBoneNames, ECollisionChannel InChannel, ECollisionResponse InNewResponse);

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    static void TraceWithPhysicsAssetBoneShapes(class UPrimitiveComponent* InMesh, const TArray<FName>& InBoneNames, ECollisionChannel TraceChannel, const TArray<AActor*>& InActorsToIgnore, TArray<FHitResult>& OutHitResults, bool InShowDebug, float ScaleMultiplier = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    static TArray<UPrimitiveComponent*> AddCollisionComponentsFromPhysicsAsset(USkeletalMeshComponent* SkeletalMesh);
};
