// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "GameplayTagContainer.h"
#include <Engine/EngineTypes.h>
#include "Engine/StaticMesh.h"
#include "TempestCombatComponent.generated.h"

UENUM(BlueprintType)
enum class EModifyingType : uint8
{
    Add			UMETA(DisplayName = "Add"),
    Remove		UMETA(DisplayName = "Remove"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifiedCombatStatus, FGameplayTag, ModifiedGameplayTag, EModifyingType, ModifiedType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedReceivedAttackProperty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedAttackProperty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnerBuffApplied, UTempestBaseBuffObject*, OwnerBuffApplied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnerBuffReapplied, UTempestBaseBuffObject*, OwnerBuffReapplied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnerBuffRemoved, UTempestBaseBuffObject*, OwnerBuffRemoved);

class UTempestBaseBuffObject;
class UTempestBaseConditionObject;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class TEMPESTCOMBATFRAMEWORK_API UTempestCombatComponent : public UTempestBaseComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UTempestCombatComponent();

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnUpdatedReceivedAttackProperty OnUpdatedReceivedAttackProperty;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnOwnerBuffReapplied OnOwnerBuffReapplied;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnOwnerBuffApplied OnOwnerBuffApplied;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnOwnerBuffRemoved OnOwnerBuffRemoved;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnUpdatedAttackProperty OnUpdatedAttackProperty;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tempest Combat Status")
    FGameplayTagContainer CombatStatusContainer;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tempest Combat Buffs")
    TArray<UTempestBaseBuffObject*> AppliedBuffs;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tempest Combat Conditions")
    TArray<UTempestBaseConditionObject*> CombatConditions;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tempest Combat Properties")
    UTempestDefensePropertiesObject* OwnerDefenseProperty;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tempest Combat Properties")
    UTempestAttackPropertiesObject* OwnerAttackProperty;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tempest Combat Properties")
    UTempestAttackPropertiesObject* ReceivedAttackProperty;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Tempest Combat Dispatchers")
    FOnModifiedCombatStatus OnModifiedCombatStatus;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tempest Combat Properties")
    TArray<AActor*> CombatTargets;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Combat Status Setters")
    void ModifyCombatStatus(FGameplayTagContainer CombatStatusToModify, EModifyingType ModifyType);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat Status Getters")
    FGameplayTagContainer GetCombatStatus() { return CombatStatusContainer; }

    UFUNCTION(BlueprintCallable, Category = "Combat Status Setters")
    void AddToAppliedBuffs(UTempestBaseBuffObject* BuffToAdd);

    UFUNCTION(BlueprintCallable, Category = "Combat Status Setters")
    void RemoveFromAppliedBuffs(UTempestBaseBuffObject* BuffToRemove);

    UFUNCTION(BlueprintCallable, Category = "Combat Status Setters")
    void SetReceivedAttackProperty(UTempestAttackPropertiesObject* NewAttackProperty);

    UFUNCTION(BlueprintCallable, Category = "Combat Status Setters")
    void SetAttackProperty(UTempestAttackPropertiesObject* NewAttackProperty);

    UFUNCTION(BlueprintCallable, Category = "Combat Status Setters")
    void SetDefenseProperty(UTempestDefensePropertiesObject* NewDefenseProperty) { OwnerDefenseProperty = NewDefenseProperty; }

    /* Construction */
    UFUNCTION(BlueprintCallable, Category = "Construction")
    void ConstructDefensePropertyOfClass(TSubclassOf<UTempestDefensePropertiesObject> PropertyToConstruct, UTempestDefensePropertiesObject*& ConstructedProperty);

    /* Getters */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
    void GetCombatTarget(AActor*& FoundCombatTarget);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
    void GetCombatTargets(TArray<AActor*>& FoundCombatTargets);

    UFUNCTION(BlueprintCallable, Category = "Combat Status Setters")
    UTempestBaseBuffObject* GetAppliedBuffOfGameplayTag(FGameplayTag InBuffTag);

    UFUNCTION(BlueprintCallable, Category = "Combat Status Setters")
    UTempestBaseBuffObject* GetAppliedBuffOfClass(TSubclassOf<UTempestBaseBuffObject> InBuffClass);
};
