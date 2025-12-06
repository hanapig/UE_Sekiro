// Created By Aamn Chahrour & Samrudh Sunil. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "TempestBaseConditionObject.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, meta = (DisplayName = "Instanced Condition Properties"))
struct FInstancedConditionProperties
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestBaseConditionObject* Condition;

    FInstancedConditionProperties()
        : Condition(nullptr) // Initialize the pointer to nullptr
    {
    }

    ~FInstancedConditionProperties()
    {
    }
};

UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCORE_API UTempestBaseConditionObject : public UTempestBaseObject
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Condition Base Functions")
    void OnConditionCreated();
    virtual void OnConditionCreated_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Condition Base Functions")
    bool GetConditionResult();
    virtual bool GetConditionResult_Implementation();
};
