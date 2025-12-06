// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_InvokeEventOfGameplayTag.generated.h"

/**
 *
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UANS_InvokeEventOfGameplayTag : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    UANS_InvokeEventOfGameplayTag();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Base Variables")
    FGameplayTag BeginEventToInvoke;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Base Variables")
    FGameplayTag EndEventToInvoke;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
