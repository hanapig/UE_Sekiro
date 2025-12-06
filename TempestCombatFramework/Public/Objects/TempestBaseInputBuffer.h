// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "Data/TempestDataContainer.h"
#include "Objects/TempestInputBufferProperty.h"
#include "Misc/Guid.h"
#include "InputAction.h"
#include "TempestBaseInputBuffer.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FBufferInfo
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Variables")
    UInputAction* AllowedInput;

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    UTempestInputBufferProperty* InputBufferSpecialProperty;

    FBufferInfo() : AllowedInput(nullptr), InputBufferSpecialProperty(nullptr) {};
    ~FBufferInfo() {};
};

UCLASS(HideDropdown)
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseInputBuffer : public UTempestBaseObject
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Parameters")
    FGameplayTag InputBufferGameplayTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Parameters", meta = (ExposeOnSpawn = "true"))
    EInputBufferType InputBufferType = EInputBufferType::HighestPriorityInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Parameters", meta = (ExposeOnSpawn = "true"))
    TArray<FBufferInfo> BufferInfo;

    UPROPERTY(BlueprintReadOnly, Category = "Base Parameters", meta = (ExposeOnSpawn = "true"))
    class UAnimInstance* AnimInstance;

    UPROPERTY(BlueprintReadOnly, Category = "Base Parameters", meta = (ExposeOnSpawn = "true"))
    class UAnimMontage* AnimMontage;

    UPROPERTY(BlueprintReadOnly, Category = "Base Parameters", meta = (ExposeOnSpawn = "true"))
    FGuid InputBufferGuid;

    UInputAction* MainBufferedInputAction;
    TArray<UInputAction*> RegisteredBufferedInputActions;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Buffer Processing")
    void ProcessInputAction(UInputAction* InputAction);
    virtual void ProcessInputAction_Implementation(UInputAction* InputAction);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Buffer Processing")
    void OnConstructedInputBuffer();
    virtual void OnConstructedInputBuffer_Implementation();

    UFUNCTION(BlueprintCallable, Category = "Input Buffer Processing")
    void SetMainBufferedInput(UInputAction* NewInputAction);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Processing")
    void GetMainBufferedInput(UInputAction*& FoundInputAction);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Processing")
    void GetRegisteredBufferedInputActions(TArray<UInputAction*>& FoundInputActions);

    UFUNCTION(BlueprintCallable, Category = "Input Buffer Processing")
    void SetRegisteredBufferedInputActions(TArray<UInputAction*> NewInputActions) { RegisteredBufferedInputActions = NewInputActions; }

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Buffer Processing")
    void AddToRegisteredBufferedInputActions(UInputAction* NewInputAction);
    virtual void AddToRegisteredBufferedInputActions_Implementation(UInputAction* NewInputAction);

    UFUNCTION(BlueprintCallable, Category = "Input Buffer Processing")
    void RemoveFromRegisteredBufferedInputActions(UInputAction* NewInputAction);

    UFUNCTION(BlueprintCallable, Category = "Input Buffer Processing")
    void ReorderArrayBasedOnPriority(TArray<UInputAction*>& ReorderedArray, const TArray<UInputAction*>& PriorityArray, const TArray<UInputAction*>& ShuffledArrayToSort);

    UFUNCTION(BlueprintCallable, Category = "Input Buffer Processing")
    void ClearRegisteredBufferedInputActions() { RegisteredBufferedInputActions.Empty(); }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Getters")
    void GetAllAllowedInputActions(TArray<UInputAction*>& FoundAllowedInputActions);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Buffer Processing")
    bool TryProcessBuffer();
    virtual bool TryProcessBuffer_Implementation();

    UFUNCTION(BlueprintCallable, Category = "Input Buffer Processing")
    void GetPropertyOfInputAction(UInputAction* InputAction, UTempestInputBufferProperty*& FoundInputBufferProperty);

    UFUNCTION(BlueprintNativeEvent)
    void HandleMontageEnded(UAnimMontage* EndedMontage, bool bInterrupted);

    UFUNCTION(BlueprintNativeEvent)
    void HandleMontageStarted(UAnimMontage* StartedMontage);
};