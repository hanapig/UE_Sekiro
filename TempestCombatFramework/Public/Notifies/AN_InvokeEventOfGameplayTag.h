// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/TempestEventsComponent.h"
#include "AN_InvokeEventOfGameplayTag.generated.h"

/**
 *
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UAN_InvokeEventOfGameplayTag : public UAnimNotify
{
    GENERATED_BODY()

public:
    UAN_InvokeEventOfGameplayTag();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Base Variables")
    FGameplayTag EventToInvoke;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
