// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RootMotionModifier.h"
#include "MotionWarpingComponent.h"
#include "CRootMotionModifier_Scale.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "Tempest Dynamic Scale"))
class TEMPESTCOMBATFRAMEWORK_API UCRootMotionModifier_Scale : public URootMotionModifier
{
    GENERATED_BODY()

    // Variables
public:

    /** Vector used to scale each component of the translation */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    FVector Scale = FVector(1.f);

    FVector UpdatedScale = FVector::ZeroVector;

    UCRootMotionModifier_Scale(const FObjectInitializer& ObjectInitializer);

    virtual FTransform ProcessRootMotion(const FTransform& InRootMotion, float DeltaSeconds) override
    {
        FTransform FinalRootMotion = InRootMotion;
        if (UpdatedScale == FVector::ZeroVector)
        {
            UpdatedScale = Scale * GetFloatPropertyValue();
        }

        FinalRootMotion.ScaleTranslation(UpdatedScale);
        return FinalRootMotion;
    }

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    float GetFloatPropertyValue();
};