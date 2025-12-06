// Fill out your copyright notice in the Description page of Project Settings.

#include "Notifies/ANS_InvokeEventOfGameplayTag.h"
#include "Components/TempestEventsComponent.h"
#include "Math/Color.h"
#include "Components/SkeletalMeshComponent.h"

UANS_InvokeEventOfGameplayTag::UANS_InvokeEventOfGameplayTag()
    : Super()
    , BeginEventToInvoke()
    , EndEventToInvoke()
{
#if WITH_EDITORONLY_DATA
    NotifyColor = FColor(255, 165, 0);
#endif // WITH_EDITORONLY_DATA
}

void UANS_InvokeEventOfGameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (BeginEventToInvoke.IsValid() && MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        if (UTempestEventsComponent* const EventsComponent = MeshComp->GetOwner()->FindComponentByClass<UTempestEventsComponent>())
        {
            EventsComponent->InvokeEventOfGameplayTag(BeginEventToInvoke);
        }
    }

    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_InvokeEventOfGameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (EndEventToInvoke.IsValid() && MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        if (UTempestEventsComponent* const EventsComponent = MeshComp->GetOwner()->FindComponentByClass<UTempestEventsComponent>())
        {
            EventsComponent->InvokeEventOfGameplayTag(EndEventToInvoke);
        }
    }

    Super::NotifyEnd(MeshComp, Animation, EventReference);
}