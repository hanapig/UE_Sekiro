// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Objects/TempestBase_AI_BehaviorObject.h"
#include "TempestPropertiesFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestPropertiesFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Duplication Base Functions", meta = (WorldContext = "WorldContextObject"))
    static FInstanced_AI_BehaviorProperty CreateDuplicateBehaviorProperty(UObject* Owner, FInstanced_AI_BehaviorProperty BehaviorPropertyToDuplicate);

    UFUNCTION(BlueprintCallable, Category = "Duplication Base Functions", meta = (WorldContext = "WorldContextObject"))
    static FInstancedAttackProperties CreateDuplicateAttackProperty(UObject* Owner, FInstancedAttackProperties AttackPropertyToDuplicate);
};