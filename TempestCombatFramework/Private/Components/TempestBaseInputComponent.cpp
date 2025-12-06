// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved. 


#include "Components/TempestBaseInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/Pawn.h"
#include "Components/TempestTickingComponent.h"

UTempestBaseInputComponent::UTempestBaseInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);
	PrimaryComponentTick.bStartWithTickEnabled = true;

	FRecordedInputActions LocalEmptyArray;
	InputActionsAndTriggerEvents.Emplace(ETriggerEvent::None, LocalEmptyArray);
	InputActionsAndTriggerEvents.Emplace(ETriggerEvent::Canceled, LocalEmptyArray);
	InputActionsAndTriggerEvents.Emplace(ETriggerEvent::Completed, LocalEmptyArray);
	InputActionsAndTriggerEvents.Emplace(ETriggerEvent::Triggered, LocalEmptyArray);
	InputActionsAndTriggerEvents.Emplace(ETriggerEvent::Ongoing, LocalEmptyArray);
	InputActionsAndTriggerEvents.Emplace(ETriggerEvent::Started, LocalEmptyArray);
}

void UTempestBaseInputComponent::AddInputBuffer(UTempestBaseInputBuffer* NewInputBuffer)
{
	if (NewInputBuffer)
	{
		ActiveInputbuffers.Emplace(NewInputBuffer);
	}
}

void UTempestBaseInputComponent::AddInputsToWatchListTag(TArray<class UInputAction*> NewInputs, FGameplayTag WatchListTag)
{
	if (WatchListTag.IsValid())
	{
		if (InputsWatchlists.Contains(WatchListTag))
		{
			FWatchListInfo* LocalWatchList;
			LocalWatchList = InputsWatchlists.Find(WatchListTag);

			for (int32 i = 0; i < NewInputs.Num(); i++)
			{
				if (NewInputs[i])
				{
					int32 LocalFoundIndex = LocalWatchList->GetInputActionTickInfoOfInputAction(NewInputs[i]);
 					if (LocalFoundIndex  == INDEX_NONE)
 					{
						FInputActionTickInfo LocalTickInfo;
						LocalTickInfo.AssignedInputAction = NewInputs[i];
 						LocalWatchList->InputActionsTickInfo.Emplace(LocalTickInfo);
 					}
				}
			}		
		}
		else
		{
			FWatchListInfo LocalWatchList;
			TArray<FInputActionTickInfo> NewInputActionsTickInfo;

			for (int32 i = 0; i < NewInputs.Num(); i++)
			{
				if (NewInputs[i])
				{
					FInputActionTickInfo LocalInputActionTickInfo;
					LocalInputActionTickInfo.AssignedInputAction = NewInputs[i];
					NewInputActionsTickInfo.Emplace(LocalInputActionTickInfo);
				}
			}

			LocalWatchList.InputActionsTickInfo = NewInputActionsTickInfo;
			InputsWatchlists.Emplace(WatchListTag, LocalWatchList);
			SetComponentTickEnabled(true);
		}
	}
}

void UTempestBaseInputComponent::RemoveInputBuffer(UTempestBaseInputBuffer* InputBufferToRemove)
{
	if (InputBufferToRemove)
	{
		if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
		{
			TickingComponent->RemoveTickableObject(InputBufferToRemove);
		}
		ActiveInputbuffers.Remove(InputBufferToRemove);
	}
}

void UTempestBaseInputComponent::ClearAllActiveInputBuffers()
{
	for (int32 i = 0; i < ActiveInputbuffers.Num(); i++)
	{
		if (ActiveInputbuffers[i])
		{
			if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
			{
				TickingComponent->RemoveTickableObject(ActiveInputbuffers[i]);
			}
		}
	}
	ActiveInputbuffers.Empty();
	CurrentProcessingInput = nullptr;
}


void UTempestBaseInputComponent::SetLastFiredBufferedInput(UInputAction* NewFiredInput)
{
	LastFiredBufferedInput = NewFiredInput;
	OnBufferedInputFired.Broadcast(LastFiredBufferedInput);
}

void UTempestBaseInputComponent::SetCurrentProcessingInput(UInputAction* NewInput)
{
	CurrentProcessingInput = NewInput;
}

void UTempestBaseInputComponent::CreateBindingsForInputActions(TArray<class UInputAction*> InInputActionsArray)
{
	if (!(InInputActionsArray.IsEmpty()))
	{
		APawn* LocalPawnOwner = Cast<APawn>(GetOwner());
		if (LocalPawnOwner != nullptr)
		{
			APlayerController* LocalPC = Cast<APlayerController>(LocalPawnOwner->GetController());
			if (LocalPC)
			{
				UEnhancedInputComponent* LocalEnhancedInputComponent = Cast<UEnhancedInputComponent>(LocalPC->InputComponent);
				for (UInputAction*& MappingElem : InInputActionsArray)
				{
					if (MappingElem)
					{
						if (!(AlreadyBoundInputActions.Contains(MappingElem)))
						{
							LocalEnhancedInputComponent->BindAction(MappingElem, ETriggerEvent::Triggered, this, &UTempestBaseInputComponent::TempestInputTriggeredEvent);
							LocalEnhancedInputComponent->BindAction(MappingElem, ETriggerEvent::Completed, this, &UTempestBaseInputComponent::TempestInputStoppedEvent);
							LocalEnhancedInputComponent->BindAction(MappingElem, ETriggerEvent::Canceled, this, &UTempestBaseInputComponent::TempestInputStoppedEvent);

							LocalEnhancedInputComponent->BindAction(MappingElem, ETriggerEvent::None, this, &UTempestBaseInputComponent::TempestInputRemainingEvents);
							LocalEnhancedInputComponent->BindAction(MappingElem, ETriggerEvent::Ongoing, this, &UTempestBaseInputComponent::TempestInputRemainingEvents);
							LocalEnhancedInputComponent->BindAction(MappingElem, ETriggerEvent::Started, this, &UTempestBaseInputComponent::TempestInputRemainingEvents);
							LocalEnhancedInputComponent->BindAction(MappingElem, ETriggerEvent::Triggered, this, &UTempestBaseInputComponent::TempestInputRemainingEvents);
							LocalEnhancedInputComponent->BindAction(MappingElem, ETriggerEvent::Completed, this, &UTempestBaseInputComponent::TempestInputRemainingEvents);
							LocalEnhancedInputComponent->BindAction(MappingElem, ETriggerEvent::Canceled, this, &UTempestBaseInputComponent::TempestInputRemainingEvents);
							AlreadyBoundInputActions.Emplace(MappingElem);
						}
					}
				}

			}
		}
		
	}
}

void UTempestBaseInputComponent::RefreshInputHistory()
{
	for (auto& Elem : InputsWatchlists)
	{
		FWatchListInfo* FoundWatchList = InputsWatchlists.Find(Elem.Key);
		if (FoundWatchList)
		{
			FoundWatchList->InputHistory.Empty();
			for (int32 i = 0; i < 5; ++i)
			{
				FInputActionHistory LocalEmptyActionHistory;
				FoundWatchList->InputHistory.Emplace(LocalEmptyActionHistory);
			}
		}
	}
}

void UTempestBaseInputComponent::SetRecordedMappingContexts(class UInputMappingContext* InMappingToRecord, FGameplayTag InMappingTag)
{
	if (InMappingToRecord != nullptr && InMappingTag.IsValid())
	{
		RecordedMappingContexts.Emplace(InMappingTag, InMappingToRecord);
	}
}

void UTempestBaseInputComponent::RemoveInputActionFromBindings(class UInputAction* InInputActionToRemove)
{
	if (InInputActionToRemove != nullptr)
	{
		if (!(AlreadyBoundInputActions.Contains(InInputActionToRemove)))
		{
			AlreadyBoundInputActions.Remove(InInputActionToRemove);
		}
	}
}

void UTempestBaseInputComponent::TempestInputTriggeredEvent(const FInputActionInstance& Instance)
{
	if (GetIsOwnerControlledByPlayerController() && AlreadyBoundInputActions.Contains(Instance.GetSourceAction()))
	{
		const UInputAction* LocalAction = Instance.GetSourceAction();
		FInputActionInstance LocalCopy(LocalAction);
		LocalAction = LocalCopy.GetSourceAction();
		UInputAction* LocalAssignedInputAction = const_cast<UInputAction*>(LocalAction);
		if (LocalAction)
		{
			TArray<FGameplayTag> LocalGameplaytags = GetWatchListTagsOfInputAction(LocalAction);

			for (FGameplayTag& Elem : LocalGameplaytags)
			{
				FWatchListInfo* FoundWatchList = InputsWatchlists.Find(Elem);
				if (FoundWatchList)
				{
					int32 FoundIndex = FoundWatchList->GetInputActionTickInfoOfInputAction(LocalAction);
					if (FoundIndex != INDEX_NONE)
					{
						FoundWatchList->InputActionsTickInfo[FoundIndex].Ticking = true;
						FoundWatchList->InputActionsTickInfo[FoundIndex].FramesTicked = 0.f;
					}

					if (!(LocalAction == FoundWatchList->CurrentTriggeredInput.AssignedInputAction))
					{
						FoundWatchList->CurrentTriggeredInput.AssignedInputAction = LocalAssignedInputAction;
						FoundWatchList->CurrentTriggeredInput.Ticking = true;
						FoundWatchList->CurrentTriggeredInput.FramesTicked = 0.f;

						FInputActionHistory LocalActionHistory;
						LocalActionHistory.AssignedInputAction = LocalAssignedInputAction;
						FoundWatchList->InputHistory.Emplace(LocalActionHistory);
						FoundWatchList->InputHistory.RemoveAt(0);

						OnWatchListCurrentTriggeredInputChanged.Broadcast(Elem, FoundWatchList->CurrentTriggeredInput.AssignedInputAction);


					}
				}
			}
		}
	}
	
}

void UTempestBaseInputComponent::TempestInputStoppedEvent(const FInputActionInstance& Instance)
{
	if (GetIsOwnerControlledByPlayerController() && AlreadyBoundInputActions.Contains(Instance.GetSourceAction()))
	{
		const UInputAction* LocalAction = Instance.GetSourceAction();
		FInputActionInstance LocalCopy(LocalAction);
		LocalAction = LocalCopy.GetSourceAction();

		if (LocalAction)
		{
			TArray<FGameplayTag> LocalGameplaytags = GetWatchListTagsOfInputAction(LocalAction);

			for (FGameplayTag& Elem : LocalGameplaytags)
			{
				FWatchListInfo* FoundWatchList = InputsWatchlists.Find(Elem);
				if (FoundWatchList)
				{
					int32 FoundIndex = FoundWatchList->GetInputActionTickInfoOfInputAction(LocalAction);
					if (FoundIndex != INDEX_NONE)
					{
						FoundWatchList->InputActionsTickInfo[FoundIndex].Ticking = false;
						FoundWatchList->InputActionsTickInfo[FoundIndex].FramesTicked = -1.f;
					}

					if (LocalAction == FoundWatchList->CurrentTriggeredInput.AssignedInputAction)
					{
						FoundWatchList->CurrentTriggeredInput.AssignedInputAction = nullptr;
						FoundWatchList->CurrentTriggeredInput.Ticking = true;
						FoundWatchList->CurrentTriggeredInput.FramesTicked = 0.f;

						FInputActionHistory LocalActionHistory;
						LocalActionHistory.AssignedInputAction = nullptr;
						FoundWatchList->InputHistory.Emplace(LocalActionHistory);
						FoundWatchList->InputHistory.RemoveAt(0);

						OnWatchListCurrentTriggeredInputChanged.Broadcast(Elem, FoundWatchList->CurrentTriggeredInput.AssignedInputAction);
					}
				}
			}
		}
	}
	
}

void UTempestBaseInputComponent::TempestInputRemainingEvents(const FInputActionInstance& Instance)
{
	if (GetIsOwnerControlledByPlayerController() && AlreadyBoundInputActions.Contains(Instance.GetSourceAction()))
	{
		const UInputAction* LocalAction = Instance.GetSourceAction();
		ETriggerEvent LocalCurrentTriggerEvent = Instance.GetTriggerEvent();
		FInputActionInstance LocalCopy(LocalAction);
		LocalAction = LocalCopy.GetSourceAction();

		if (LocalAction)
		{
			if (InputActionsAndTriggerEvents.Contains(LocalCurrentTriggerEvent))
			{
				UInputAction* NonConstantInputAction = const_cast<UInputAction*>(LocalAction);
				FRecordedInputActions* LocalRecordedInputActions;
				LocalRecordedInputActions = InputActionsAndTriggerEvents.Find(LocalCurrentTriggerEvent);
				if (!(LocalRecordedInputActions->AssignedInputActions.Contains(LocalAction)))
				{
					for (auto& Elem : InputActionsAndTriggerEvents)
					{
						if (Elem.Value.AssignedInputActions.Contains(LocalAction))
						{
							Elem.Value.AssignedInputActions.Remove(NonConstantInputAction);
						}
					}
					if (!(LocalCurrentTriggerEvent == ETriggerEvent::None))
					{
						LocalRecordedInputActions->AssignedInputActions.Emplace(NonConstantInputAction);
					}
				}
			}
		}
		OnInputTriggerEventChange.Broadcast(Instance);
	}
	
}


void UTempestBaseInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetIsOwnerControlledByPlayerController())
	{
		if (!(InputsWatchlists.IsEmpty()))
		{
			for (TMap<FGameplayTag, FWatchListInfo>::TIterator It(InputsWatchlists); It; ++It)
			{
				const FGameplayTag& Tag = It.Key();
				FWatchListInfo& Info = It.Value();

				TArray<FInputActionTickInfo> LocalInputActionsTickInfo = Info.InputActionsTickInfo;
				for (int32 i = 0; i < LocalInputActionsTickInfo.Num(); ++i)
				{
					if (LocalInputActionsTickInfo[i].Ticking)
					{
						LocalInputActionsTickInfo[i].FramesTicked += DeltaTime;
					}
				}

				if (Info.CurrentTriggeredInput.Ticking)
				{
					Info.CurrentTriggeredInput.FramesTicked += DeltaTime;

					if (!(Info.InputHistory.IsEmpty()))
					{
						if (Info.InputHistory.Last().AssignedInputAction == Info.CurrentTriggeredInput.AssignedInputAction)
						{
							Info.InputHistory.Last().FramesTicked = Info.CurrentTriggeredInput.FramesTicked;
						}
					}
				}
			}
		}
		else
		{
			SetComponentTickEnabled(false);
		}
	}
	
}

void UTempestBaseInputComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
	ClearAllActiveInputBuffers();
}

class UInputMappingContext* UTempestBaseInputComponent::GetMappingContextOfTag(FGameplayTag MappingContextTag)
{
	UInputMappingContext* LocalMappingContext = nullptr;
	for (const TPair<FGameplayTag,UInputMappingContext* >& Elem : RecordedMappingContexts)
	{
		if (Elem.Key == MappingContextTag)
		{
			return Elem.Value;
		}
	}
	return LocalMappingContext;
}

bool UTempestBaseInputComponent::GetIsOwnerControlledByPlayerController()
{
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (LocalPlayerController)
	{
		APawn* LocalControlledPawn = LocalPlayerController->GetPawn();
		return LocalControlledPawn == GetOwner();
	}
	return false;
}

bool UTempestBaseInputComponent::GetDoesInputActionExistInBoundInputs(class UInputAction* InInputAction)
{
	if (InInputAction != nullptr)
	{
		return AlreadyBoundInputActions.Contains(InInputAction);
	}
	return false;
}

void UTempestBaseInputComponent::ProcessInputAction_Implementation(bool& bCanProceed, UInputAction* InputAction)
{
	bCanProceed = true;
}


void UTempestBaseInputComponent::GetInputBufferOfGameplayTag(FGameplayTag GameplayTagRequired, UTempestBaseInputBuffer*& FoundInputBuffer)
{
	if (ActiveInputbuffers.Num() > 0)
	{
		for (UTempestBaseInputBuffer*& Elem : ActiveInputbuffers)
		{
			if (Elem)
			{
				if (Elem->InputBufferGameplayTag == GameplayTagRequired)
				{
					FoundInputBuffer = Elem;
					break;
				}
			}
		}
	}
}

void UTempestBaseInputComponent::GetInputBufferOfClass(TSubclassOf<UTempestBaseInputBuffer> InputBufferClassRequired, UTempestBaseInputBuffer*& FoundInputBuffer)
{
	if (ActiveInputbuffers.Num() > 0 && InputBufferClassRequired)
	{
		for (UTempestBaseInputBuffer*& Elem : ActiveInputbuffers)
		{
			if (Elem)
			{
				if (Elem->GetClass() == InputBufferClassRequired)
				{
					FoundInputBuffer = Elem;
					break;
				}
			}
		}
	}
}

void UTempestBaseInputComponent::GetInputBufferOfType(EInputBufferType BufferTypeRequired, UTempestBaseInputBuffer*& FoundInputBuffer)
{
	if (ActiveInputbuffers.Num() > 0)
	{
		for (UTempestBaseInputBuffer*& Elem : ActiveInputbuffers)
		{
			if (Elem)
			{
				if (Elem->InputBufferType == BufferTypeRequired)
				{
					FoundInputBuffer = Elem;
					break;
				}
			}
		}
	}
}

void UTempestBaseInputComponent::GetInputBufferOfGuid(FGuid BufferGuidRequired, UTempestBaseInputBuffer*& FoundInputBuffer)
{
	if (ActiveInputbuffers.Num() > 0)
	{
		for (UTempestBaseInputBuffer*& Elem : ActiveInputbuffers)
		{
			if (Elem)
			{
				if (Elem->InputBufferGuid == BufferGuidRequired)
				{
					FoundInputBuffer = Elem;
					break;
				}
			}
		}
	}
}

void UTempestBaseInputComponent::GetInputBuffersOfInputAction(UInputAction* InputAction, TArray<UTempestBaseInputBuffer*>& FoundInputBuffers)
{
	if (ActiveInputbuffers.Num() > 0)
	{
		for (UTempestBaseInputBuffer*& Elem : ActiveInputbuffers)
		{
			if (Elem)
			{
				for (FBufferInfo& Elem2 : Elem->BufferInfo)
				{
					if (Elem2.AllowedInput == InputAction)
					{
						FoundInputBuffers.Emplace(Elem);
					}
				}
			}
		}
	}
}

TArray<FGameplayTag> UTempestBaseInputComponent::GetWatchListTagsOfInputAction(const UInputAction* InInputAction)
{
	TArray<FGameplayTag> LocalGameplaytags;
	if (!(InputsWatchlists.IsEmpty()))
	{
		for (TMap<FGameplayTag, FWatchListInfo>::TIterator It(InputsWatchlists); It; ++It)
		{
			const FGameplayTag& Tag = It.Key();
			FWatchListInfo& Info = It.Value(); 

			int32 FoundIndex = Info.GetInputActionTickInfoOfInputAction(InInputAction);
			if (FoundIndex != INDEX_NONE)
			{
				LocalGameplaytags.Emplace(Tag);
			}
		}
	}

	return LocalGameplaytags;
	
}

ETriggerEvent UTempestBaseInputComponent::GetInputActionCurrentTriggerEvent(const UInputAction* InInputAction)
{
	if (InInputAction)
	{
		for (auto& Elem : InputActionsAndTriggerEvents)
		{
			if (Elem.Value.AssignedInputActions.Contains(InInputAction))
			{
				return Elem.Key;
			}
		}
	}

	return ETriggerEvent::None;
}
