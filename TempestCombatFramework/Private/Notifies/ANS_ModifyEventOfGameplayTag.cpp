// Fill out your copyright notice in the Description page of Project Settings.

#include "Notifies/ANS_ModifyEventOfGameplayTag.h"
#include "Math/Color.h"
#include "Components/SkeletalMeshComponent.h"

UANS_ModifyEventOfGameplayTag::UANS_ModifyEventOfGameplayTag()
    : Super()
    , BeginEventToModify()
    , BeginEventModifyType(EEventModifyType::Add)
    , EndEventToModify()
    , EndEventModifyType(EEventModifyType::Remove)
{
#if WITH_EDITORONLY_DATA
    NotifyColor = FColor(200, 100, 0);
#endif // WITH_EDITORONLY_DATA
}

void UANS_ModifyEventOfGameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (BeginEventToModify.IsValid() && MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        if (UTempestEventsComponent* const EventsComponent = MeshComp->GetOwner()->FindComponentByClass<UTempestEventsComponent>())
        {
            EventsComponent->ModifyEventOfGameplayTag(BeginEventToModify, BeginEventModifyType);
        }
    }

    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_ModifyEventOfGameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (EndEventToModify.IsValid() && MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        if (UTempestEventsComponent* const EventsComponent = MeshComp->GetOwner()->FindComponentByClass<UTempestEventsComponent>())
        {
            EventsComponent->ModifyEventOfGameplayTag(EndEventToModify, EndEventModifyType);
        }
    }

    Super::NotifyEnd(MeshComp, Animation, EventReference);
}