// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "TempestDebugObject.generated.h"

/**
 *
 */

class UTempestBaseAbilityManagerComponent;
class UTempestCombatComponent;
class UTempestBaseStateManagerComponent;
class UTempestTickingComponent;

UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestDebugObject : public UObject
{
    GENERATED_BODY()
    //Functions
public:
    /** Constructor */
    UTempestDebugObject();

    UPROPERTY()
    UTempestBaseAbilityManagerComponent* AbilityComponent;
    UPROPERTY()
    UTempestCombatComponent* CombatComponent;
    UPROPERTY()
    UTempestBaseStateManagerComponent* StateComponent;
    UPROPERTY()
    UTempestTickingComponent* TickComponent;

    /** Function to Start/Update debugging */
    UFUNCTION(BlueprintCallable, Category = "Debug Base Functions")
    void UpdateDebugging(class APawn* ControlledPawn);

    /** Function to start the info for debugging */
    void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);

    UFUNCTION()
    void SaveComponentsReferences(class APawn* ControlledPawn);
};
