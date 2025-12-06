// Fill out your copyright notice in the Description page of Project Settings.

#include "Notifies/AN_InvokeEventOfGameplayTag.h"
#include "Math/Color.h"
#include "Components/SkeletalMeshComponent.h"

UAN_InvokeEventOfGameplayTag::UAN_InvokeEventOfGameplayTag()
    : Super()
    , EventToInvoke()
{
#if WITH_EDITORONLY_DATA
    NotifyColor = FColor(255, 165, 0);
#endif // WITH_EDITORONLY_DATA
}

void UAN_InvokeEventOfGameplayTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (EventToInvoke.IsValid() && MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        if (UTempestEventsComponent* const EventsComponent = MeshComp->GetOwner()->FindComponentByClass<UTempestEventsComponent>())
        {
            EventsComponent->InvokeEventOfGameplayTag(EventToInvoke);
        }
    }

    Super::Notify(MeshComp, Animation, EventReference);
}