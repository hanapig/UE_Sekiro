// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "Engine/EngineTypes.h"
#include "Engine/HitResult.h"
#include "Objects/TempestBaseFeelObject.h"
#include "Objects/TempestBaseImpactObject.h"
#include "Objects/TempestAttackTraitObject.h"
#include "GameplayTagContainer.h"
#include "TempestAttackPropertiesObject.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitResultUpdated, FHitResult, NewHitResult);

USTRUCT(BlueprintType, meta = (DisplayName = "Instanced Attack Properties"))
struct FInstancedAttackProperties
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestAttackPropertiesObject* AttackProperty;

    FInstancedAttackProperties() : AttackProperty(nullptr) {};
    ~FInstancedAttackProperties() {};
};

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew)
class TEMPESTCOMBATFRAMEWORK_API UTempestAttackPropertiesObject : public UTempestBaseObject
{
    GENERATED_BODY()

public:
    UTempestAttackPropertiesObject();

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnHitResultUpdated OnHitResultUpdated;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnAttackComplete OnAttackComplete;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Property Initialization")
    FGameplayTag AttackPropertyTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Property Initialization")
    TArray<FInstancedAttackTrait> AttackPropertyTraits;

    UPROPERTY()
    FHitResult HitResult;

    UPROPERTY()
    FGameplayTag ImpactResult;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact")
    TArray<FInstancedImpactProperties> ImpactProperties;

protected:

    UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
    AActor* AttackPropertyOwner;

    UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
    AActor* AttackPropertyInstigator;

    UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
    class UTempestBaseTraceObject* HitTraceObject;

public:

    UFUNCTION(BlueprintPure, Category = "Base Functions")
    AActor* GetAttackPropertyOwner();

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetAttackPropertyOwner(AActor* NewAttacker);

    UFUNCTION(BlueprintPure, Category = "Base Functions")
    AActor* GetAttackPropertyInstigator();

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetAttackPropertyInstigator(AActor* NewAttackerInstigator);

    UFUNCTION(BlueprintPure, Category = "Base Functions")
    class UTempestBaseTraceObject* GetHitAttackTraceOwner();

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetHitAttackTrace(class UTempestBaseTraceObject* InHitTrace);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Base Functions")
    FORCEINLINE FHitResult GetHitResult() { return HitResult; }

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetHitResult(FHitResult NewHitResult);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Base Functions")
    FORCEINLINE FGameplayTag GetImpactResult() { return ImpactResult; }

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetImpactResult(FGameplayTag NewImpactResult);

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void AssignImpactProperties();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ApplyImpactProperties();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ConstructAttackProperty();
    virtual void ConstructAttackProperty_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ProcessAttackProperty();
    virtual void ProcessAttackProperty_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ProcessAttackTraits();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Base Functions")
    void GetImpactPropertyOfGameplayTag(FInstancedImpactProperties& PropertyFound, FGameplayTag PropertyToFind);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Base Functions | Getters")
    UTempestAttackTraitObject* GetAttackTraitOfGameplayTag(FGameplayTag InGameplayTag);
};
