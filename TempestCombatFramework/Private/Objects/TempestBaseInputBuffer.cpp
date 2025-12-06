// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved.

#include "Objects/TempestBaseInputBuffer.h"
#include "Components/TempestBaseInputComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UTempestBaseInputBuffer::ProcessInputAction_Implementation(UInputAction* InputAction)
{
}

void UTempestBaseInputBuffer::OnConstructedInputBuffer_Implementation()
{
    if (AnimInstance != nullptr && AnimMontage != nullptr)
    {                                                                                                                                                                                                                                                      
        AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &UTempestBaseInputBuffer::HandleMontageEnded);
        AnimInstance->OnMontageStarted.AddUniqueDynamic(this, &UTempestBaseInputBuffer::HandleMontageStarted);
    }
}

void UTempestBaseInputBuffer::SetMainBufferedInput(UInputAction* NewInputAction)
{
    MainBufferedInputAction = NewInputAction;
}

void UTempestBaseInputBuffer::GetMainBufferedInput(UInputAction*& FoundInputAction)
{
    FoundInputAction = MainBufferedInputAction;
}

void UTempestBaseInputBuffer::GetRegisteredBufferedInputActions(TArray<UInputAction*>& FoundInputActions)
{
    FoundInputActions = RegisteredBufferedInputActions;
}

void UTempestBaseInputBuffer::AddToRegisteredBufferedInputActions_Implementation(UInputAction* NewInputAction)
{
    if (NewInputAction)
    {
        if (InputBufferType == EInputBufferType::LastRegisteredInput)
        {
            if (RegisteredBufferedInputActions.Contains(NewInputAction))
            {
                RegisteredBufferedInputActions.Remove(NewInputAction);
                RegisteredBufferedInputActions.Insert(NewInputAction, 0);
                return;
            }
            else
            {
                RegisteredBufferedInputActions.Insert(NewInputAction, 0);
                return;
            }
        }

        RegisteredBufferedInputActions.AddUnique(NewInputAction);
    }
}

void UTempestBaseInputBuffer::RemoveFromRegisteredBufferedInputActions(UInputAction* NewInputAction)
{
    if (NewInputAction)
    {
        RegisteredBufferedInputActions.Remove(NewInputAction);
    }
}

void UTempestBaseInputBuffer::ReorderArrayBasedOnPriority(TArray<UInputAction*>& ReorderedArray, const TArray<UInputAction*>& PriorityArray, const TArray<UInputAction*>& ShuffledArrayToSort)
{
    // Mapping of priorities based on the order in PriorityArray
    TMap<UInputAction*, int32> PriorityMap;
    for (int32 i = 0; i < PriorityArray.Num(); ++i)
    {
        PriorityMap.Emplace(PriorityArray[i], i);
    }

    ReorderedArray = ShuffledArrayToSort;
    ReorderedArray.Sort([&](const UInputAction& A, const UInputAction& B)
        {
            // Get the priority of each action, defaulting to a large number if not found to ensure they end up at the end of the list
            int32 PriorityA = PriorityMap.Contains(&A) ? PriorityMap[&A] : MAX_int32;
            int32 PriorityB = PriorityMap.Contains(&B) ? PriorityMap[&B] : MAX_int32;

            // Compare the priorities
            return PriorityA < PriorityB;
        });
}

void UTempestBaseInputBuffer::GetAllAllowedInputActions(TArray<UInputAction*>& FoundAllowedInputActions)
{
    if (BufferInfo.Num() > 0)
    {
        for (FBufferInfo& Elem : BufferInfo)
        {
            FoundAllowedInputActions.Emplace(Elem.AllowedInput);
        }
    }
}

bool UTempestBaseInputBuffer::TryProcessBuffer_Implementation()
{
    if (MainBufferedInputAction)
    {
        UTempestInputBufferProperty* LocalInputBufferProperty;
        GetPropertyOfInputAction(MainBufferedInputAction, LocalInputBufferProperty);
        if (LocalInputBufferProperty)
        {
            LocalInputBufferProperty->SetInputToProcess(MainBufferedInputAction);
            LocalInputBufferProperty->SetPropertyOwner(GetOuter());
            AActor* LocalActor = Cast<AActor>(GetOuter());
            if (LocalActor)
            {
                UTempestBaseInputComponent* InputComponent = LocalActor->FindComponentByClass<UTempestBaseInputComponent>();
                if (InputComponent)
                {
                    InputComponent->SetCurrentProcessingInput(MainBufferedInputAction);
                    if (LocalInputBufferProperty->GetCanProcessProperty())
                    {
                        InputComponent->SetLastFiredBufferedInput(MainBufferedInputAction);
                        LocalInputBufferProperty->ProcessSpecialProperty();
                        InputComponent->SetCurrentProcessingInput(nullptr);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void UTempestBaseInputBuffer::GetPropertyOfInputAction(UInputAction* InputAction, UTempestInputBufferProperty*& FoundInputBufferProperty)
{
    if (InputAction)
    {
        for (FBufferInfo& Elem : BufferInfo)
        {
            if (Elem.AllowedInput == InputAction)
            {
                FoundInputBufferProperty = Elem.InputBufferSpecialProperty;
                break;
            }
        }
    }
}

void UTempestBaseInputBuffer::HandleMontageEnded_Implementation(UAnimMontage* EndedMontage, bool bInterrupted)
{
    if (bInterrupted && EndedMontage == AnimMontage)
    {
        if (AActor* const Owner = Cast<AActor>(GetOuter()))
        {
            if (UTempestBaseInputComponent* const TempestInputComponent = Owner->FindComponentByClass<UTempestBaseInputComponent>())
            {
                UE_LOG(LogTemp, Log, TEXT("Buffer Ended: %s"), *this->GetName());
                TempestInputComponent->RemoveInputBuffer(this);
            }

            if (AnimInstance != nullptr && AnimMontage != nullptr)
            {
                AnimInstance->OnMontageEnded.RemoveDynamic(this, &UTempestBaseInputBuffer::HandleMontageEnded);
            }
        }
    }
}

void UTempestBaseInputBuffer::HandleMontageStarted_Implementation(UAnimMontage* StartedMontage)
{
    if (StartedMontage != nullptr)
    {
        if (AActor* const Owner = Cast<AActor>(GetOuter()))
        {
            if (UTempestBaseInputComponent* const TempestInputComponent = Owner->FindComponentByClass<UTempestBaseInputComponent>())
            {
                UE_LOG(LogTemp, Log, TEXT("Buffer STARTED: %s"), *this->GetName());
                TempestInputComponent->RemoveInputBuffer(this);
            }

            if (AnimInstance != nullptr && AnimMontage != nullptr)
            {
                AnimInstance->OnMontageEnded.RemoveDynamic(this, &UTempestBaseInputBuffer::HandleMontageEnded);
                AnimInstance->OnMontageStarted.RemoveDynamic(this, &UTempestBaseInputBuffer::HandleMontageStarted);
            }
        }
    }
}