// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "GameplayTagContainer.h"
#include "TempestEventsComponent.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class EEventModifyType : uint8
{
    Add			UMETA(DisplayName = "Add"),
    Remove		UMETA(DisplayName = "Remove"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOwnerEventModified, FGameplayTag, InEventToModify, EEventModifyType, InEventModifyType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnerEventInvoked, FGameplayTag, InInvokedEvent);

UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestEventsComponent : public UTempestBaseComponent
{
    GENERATED_BODY()

public:

    UTempestEventsComponent();

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnOwnerEventModified OnOwnerEventModified;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnOwnerEventInvoked OnOwnerEventInvoked;

    UPROPERTY()
    FGameplayTagContainer CurrentEvents;

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void InvokeEventOfGameplayTag(FGameplayTag InGameplayTagToInvoke);

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void ModifyEventOfGameplayTag(FGameplayTag InGameplayTagToInvoke, EEventModifyType InInvokeType);

    UFUNCTION(BlueprintPure, Category = "Base Functions")
    FGameplayTagContainer GetCurrentEvents();
};