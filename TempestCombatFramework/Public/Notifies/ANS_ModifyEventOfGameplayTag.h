// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/TempestEventsComponent.h"
#include "GameplayTagContainer.h"
#include "ANS_ModifyEventOfGameplayTag.generated.h"

/**
 *
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UANS_ModifyEventOfGameplayTag : public UAnimNotifyState
{
    GENERATED_BODY()

public:

    UANS_ModifyEventOfGameplayTag();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Base Variables")
    FGameplayTag BeginEventToModify;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Base Variables")
    EEventModifyType BeginEventModifyType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Base Variables")
    FGameplayTag EndEventToModify;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Base Variables")
    EEventModifyType EndEventModifyType;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
