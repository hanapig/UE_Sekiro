// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "Data/TempestDataContainer.h"
#include "TempestBaseFeelObject.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, meta = (DisplayName = "Instanced Feel Properties"))
struct FInstancedFeelProperties
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestBaseFeelObject* FeelObject;

    FInstancedFeelProperties() : FeelObject(nullptr) {};
    ~FInstancedFeelProperties() {};
};

UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseFeelObject : public UTempestBaseObject
{
    GENERATED_BODY()

public:
    UTempestBaseFeelObject();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
    FGameplayTag FeelGameplayTag;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Initialization")
    EActivationStatus FeelActivationStatus = EActivationStatus::None;

    UPROPERTY()
    AActor* FeelOwner;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Base Functions")
    AActor* GetFeelOwner() { return FeelOwner; }

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetFeelOwner(AActor* NewFeelOwner);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Feel Object Base Functions")
    void ConstructFeelObject();
    virtual void ConstructFeelObject_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Feel Object Base Functions")
    void ActivateFeelObject();
    virtual void ActivateFeelObject_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Feel Object Base Functions")
    void DeactivateFeelObject();
    virtual void DeactivateFeelObject_Implementation();
};
