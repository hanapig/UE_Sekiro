// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "Objects/TempestBaseInputBuffer.h"
#include "Data/TempestDataContainer.h"
#include "InputAction.h"
#include "TempestBaseInputComponent.generated.h"

/**
 * 
 */

class UTempestBaseInputBuffer;

USTRUCT(BlueprintType)
struct FInputActionTickInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	class UInputAction* AssignedInputAction = nullptr;

	UPROPERTY()
	bool Ticking = false;

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	float FramesTicked = -1.f;

	FInputActionTickInfo() {};
};

USTRUCT(BlueprintType)
struct FInputActionHistory
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	class UInputAction* AssignedInputAction = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	float FramesTicked = 0.f;

	FInputActionHistory() {};
};

USTRUCT(BlueprintType)
struct FRecordedInputActions
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	TArray<class UInputAction*> AssignedInputActions;

	FRecordedInputActions() {};
};

USTRUCT(BlueprintType)
struct FWatchListInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	TArray<FInputActionTickInfo> InputActionsTickInfo;

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	FInputActionTickInfo CurrentTriggeredInput;

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	TArray<FInputActionHistory> InputHistory;

	int32 GetInputActionTickInfoOfInputAction(const UInputAction* InInputAction) 
	{
		for (int32 i = 0; i < InputActionsTickInfo.Num(); i++)
		{
			if (InputActionsTickInfo[i].AssignedInputAction == InInputAction)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	FWatchListInfo()
	{
		CurrentTriggeredInput.Ticking = true;
		CurrentTriggeredInput.FramesTicked = 0.f;

		for (int32 i = 0; i < 5; ++i)
		{
			FInputActionHistory LocalEmptyActionHistory;
			InputHistory.Emplace(LocalEmptyActionHistory);
		}
		
	};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBufferedFiredInput, UInputAction*, FiredBufferedInput);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputTriggerEventChanged, FInputActionInstance, InputActionInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWatchListCurrentTriggeredInputChanged, FGameplayTag, WatchListTag, UInputAction*, ActiveInputAction);

UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseInputComponent : public UTempestBaseComponent
{
	GENERATED_BODY()
public:

	UTempestBaseInputComponent();

	UPROPERTY()
	TArray<UTempestBaseInputBuffer*> ActiveInputbuffers;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Variables")
	FOnBufferedFiredInput OnBufferedInputFired;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Variables")
	FOnInputTriggerEventChanged OnInputTriggerEventChange;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Variables")
	FOnWatchListCurrentTriggeredInputChanged OnWatchListCurrentTriggeredInputChanged;

	UPROPERTY()
	TMap<FGameplayTag, class UInputMappingContext*> RecordedMappingContexts;

protected:

	UPROPERTY()
	UInputAction* LastFiredBufferedInput;

	UPROPERTY()
	UInputAction* CurrentProcessingInput;

	UPROPERTY()
	TMap<ETriggerEvent, FRecordedInputActions> InputActionsAndTriggerEvents;

	UPROPERTY()
	TMap <FGameplayTag, FWatchListInfo> InputsWatchlists;

	UPROPERTY()
	TArray<class UInputAction*> AlreadyBoundInputActions;

public:
					/*Getters*/

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Component | Getters")
	UInputAction* GetLastFiredBufferedInput() {return LastFiredBufferedInput;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Component | Getters")
	UInputAction* GetCurrentProcessingInput() {return CurrentProcessingInput;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Component | Getters")
	void GetActiveInputBuffers(TArray<UTempestBaseInputBuffer*>& CurrentActiveBuffers)const { CurrentActiveBuffers = ActiveInputbuffers;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Component | Getters")
	void GetInputBufferOfGameplayTag(FGameplayTag GameplayTagRequired, UTempestBaseInputBuffer*& FoundInputBuffer);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Component | Getters")
	void GetInputBufferOfClass(TSubclassOf<UTempestBaseInputBuffer> InputBufferClassRequired, UTempestBaseInputBuffer*& FoundInputBuffer);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Component | Getters")
	void GetInputBufferOfType(EInputBufferType BufferTypeRequired, UTempestBaseInputBuffer*& FoundInputBuffer);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input Buffer Component | Getters")
	void GetInputBufferOfGuid(FGuid BufferGuidRequired, UTempestBaseInputBuffer*& FoundInputBuffer);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Getters")
	void GetInputBuffersOfInputAction(UInputAction* InputAction, TArray<UTempestBaseInputBuffer*>& FoundInputBuffers);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Getters")
	TArray<FGameplayTag> GetWatchListTagsOfInputAction(const UInputAction* InInputAction);

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Input Buffer Component | Getters")
	ETriggerEvent GetInputActionCurrentTriggerEvent(const UInputAction* InInputAction);

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Input Buffer Component | Getters")
	TMap <FGameplayTag, FWatchListInfo> GetInputsWatchLists() {return InputsWatchlists;};

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Input Buffer Component | Getters")
	TMap<ETriggerEvent, FRecordedInputActions> GetInputActionsAndTriggerEvents() {return InputActionsAndTriggerEvents;};

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Input Buffer Component | Getters")
	TMap<FGameplayTag, class UInputMappingContext*> GetRecordedMappingContexts() { return RecordedMappingContexts; };

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Input Buffer Component | Getters")
	class UInputMappingContext* GetMappingContextOfTag(FGameplayTag MappingContextTag);

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Input Buffer Component | Getters")
	bool GetIsOwnerControlledByPlayerController();

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Input Buffer Component | Getters")
	bool GetDoesInputActionExistInBoundInputs(class UInputAction* InInputAction);

					/* Process */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Input Buffer Component | Processing")
	void ProcessInputAction(bool& bCanProceed, UInputAction* InputAction);
	virtual void ProcessInputAction_Implementation(bool& bCanProceed, UInputAction* InputAction);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Processing")
	void AddInputBuffer(UTempestBaseInputBuffer* NewInputBuffer);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Processing")
	void AddInputsToWatchListTag(TArray<class UInputAction*> NewInputs, FGameplayTag WatchListTag);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Processing")
	void RemoveInputBuffer(UTempestBaseInputBuffer* InputBufferToRemove);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Processing")
	void ClearAllActiveInputBuffers();

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Processing")
	void TempestInputTriggeredEvent(const FInputActionInstance& Instance);

	
					/* Setters */

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Setters")
	void SetLastFiredBufferedInput(UInputAction* NewFiredInput);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Setters")
	void SetCurrentProcessingInput(UInputAction* NewInput);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Setters")
	void CreateBindingsForInputActions(TArray<class UInputAction*> InInputActionsArray);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Setters")
	void RefreshInputHistory();

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Setters")
	void SetRecordedMappingContexts(class UInputMappingContext* InMappingToRecord, FGameplayTag InMappingTag);

	UFUNCTION(BlueprintCallable, Category = "Input Buffer Component | Setters")
	void RemoveInputActionFromBindings(class UInputAction* InInputActionToRemove);

	void TempestInputStoppedEvent(const FInputActionInstance& Instance);
	void TempestInputRemainingEvents(const FInputActionInstance& Instance);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnOwnerDestroyed(AActor* DestroyedActor) override;
	
};
