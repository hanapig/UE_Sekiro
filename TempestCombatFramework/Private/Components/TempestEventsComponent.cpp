// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TempestEventsComponent.h"

UTempestEventsComponent::UTempestEventsComponent()
    : Super()
    , CurrentEvents()
{
}

void UTempestEventsComponent::InvokeEventOfGameplayTag(FGameplayTag InGameplayTagToInvoke)
{
    OnOwnerEventInvoked.Broadcast(InGameplayTagToInvoke);
}

void UTempestEventsComponent::ModifyEventOfGameplayTag(FGameplayTag InGameplayTagToModify, EEventModifyType InModifyType)
{
    if (InModifyType == EEventModifyType::Add)
    {
        if (!CurrentEvents.HasTagExact(InGameplayTagToModify))
        {
            CurrentEvents.AddTag(InGameplayTagToModify);
            OnOwnerEventModified.Broadcast(InGameplayTagToModify, EEventModifyType::Add);
        }
    }
    else
    {
        if (CurrentEvents.HasTagExact(InGameplayTagToModify))
        {
            CurrentEvents.RemoveTag(InGameplayTagToModify);
            OnOwnerEventModified.Broadcast(InGameplayTagToModify, EEventModifyType::Remove);
        }
    }
}

FGameplayTagContainer UTempestEventsComponent::GetCurrentEvents()
{
    return CurrentEvents;
}