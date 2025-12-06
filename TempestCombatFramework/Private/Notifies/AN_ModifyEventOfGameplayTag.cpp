// Fill out your copyright notice in the Description page of Project Settings.

#include "Notifies/AN_ModifyEventOfGameplayTag.h"
#include "Math/Color.h"
#include "Components/SkeletalMeshComponent.h"

UAN_ModifyEventOfGameplayTag::UAN_ModifyEventOfGameplayTag()
    : Super()
    , EventToModify()
    , EventModifyType(EEventModifyType::Add)
{
#if WITH_EDITORONLY_DATA
    NotifyColor = FColor(200, 100, 0);
#endif // WITH_EDITORONLY_DATA
}

void UAN_ModifyEventOfGameplayTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (EventToModify.IsValid() && MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        if (UTempestEventsComponent* const EventsComponent = MeshComp->GetOwner()->FindComponentByClass<UTempestEventsComponent>())
        {
            EventsComponent->ModifyEventOfGameplayTag(EventToModify, EventModifyType);
        }
    }

    Super::Notify(MeshComp, Animation, EventReference);
}