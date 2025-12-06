// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "GameplayTagContainer.h"
#include "TempestBaseImpactObject.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, meta = (DisplayName = "Instanced Impact Properties"))
struct FInstancedImpactProperties
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestBaseImpactObject* ImpactProperty;

    FInstancedImpactProperties() : ImpactProperty(nullptr) {};
    ~FInstancedImpactProperties() {};
};

UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseImpactObject : public UTempestBaseObject
{
    GENERATED_BODY()
public:
    UTempestBaseImpactObject();

    UPROPERTY()
    UTempestAttackPropertiesObject* AttackProperty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Property Initialization")
    FGameplayTag ImpactPropertyTag;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ProcessImpactProperty();
    virtual void ProcessImpactProperty_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ApplyImpactProperty();
    virtual void ApplyImpactProperty_Implementation();

    UFUNCTION()
    void SetAttackProperty(UTempestAttackPropertiesObject* NewAttackProperty) { AttackProperty = NewAttackProperty; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Impact Property Getters")
    void GetAttackProperty(UTempestAttackPropertiesObject*& FoundAttackProperty) { FoundAttackProperty = AttackProperty; }
};
