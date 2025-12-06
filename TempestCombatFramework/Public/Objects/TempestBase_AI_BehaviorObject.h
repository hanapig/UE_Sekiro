// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "GameFramework/Actor.h"
#include "TempestBase_AI_BehaviorObject.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, meta = (DisplayName = "Instanced AI Behavior Property"))
struct FInstanced_AI_BehaviorProperty
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestBase_AI_BehaviorObject* AI_BehaviorToCreate;

    FInstanced_AI_BehaviorProperty() : AI_BehaviorToCreate(nullptr) {};
    ~FInstanced_AI_BehaviorProperty() {};
};
UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCOMBATFRAMEWORK_API UTempestBase_AI_BehaviorObject : public UTempestBaseObject
{
    GENERATED_BODY()
public:
    UTempestBase_AI_BehaviorObject();

    UPROPERTY()
    AActor* BehaviorOwner;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void Process_AI_Behavior();
    virtual void Process_AI_Behavior_Implementation();

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetBehaviorOwner(AActor* NewOwner) { BehaviorOwner = NewOwner; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Base Functions")
    AActor* GetBehaviorOwner() { return BehaviorOwner; }
};
