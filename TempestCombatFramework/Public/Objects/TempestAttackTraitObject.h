// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "TempestAttackTraitObject.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "Instanced Attack Trait"))
struct FInstancedAttackTrait
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestAttackTraitObject* AttackTraitProperty;

    FInstancedAttackTrait() : AttackTraitProperty(nullptr) {};
    ~FInstancedAttackTrait() {};
};

/**
 *
 */
UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCOMBATFRAMEWORK_API UTempestAttackTraitObject : public UTempestBaseObject
{
    GENERATED_BODY()

public:
    UTempestAttackTraitObject();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Variables")
    FGameplayTag TraitTag;

    UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
    class UTempestAttackPropertiesObject* AttackProperty;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ProcessAttackTrait();
};

UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestPushMultiplier : public UTempestAttackTraitObject
{
    GENERATED_BODY()

public:
    UTempestPushMultiplier();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Variables")
    float PushMultiplier;
};