// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "TempestBaseRotationProperty.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, meta = (DisplayName = "Instanced Spawner Processor"))
struct FInstancedRotationProperty
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestBaseRotationProperty* RotationProperty;

    FInstancedRotationProperty() : RotationProperty(nullptr) {};
    ~FInstancedRotationProperty() {};
};

UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseRotationProperty : public UTempestBaseObject
{
    GENERATED_BODY()
public:
    UTempestBaseRotationProperty();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Category = "Base Functions")
    bool GetCanRotate();
    virtual bool GetCanRotate_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void StartRotationProperty();
    virtual void StartRotationProperty_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void EndRotationProperty();
    virtual void EndRotationProperty_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void PerformRotationProperty();
    virtual void PerformRotationProperty_Implementation();
};
