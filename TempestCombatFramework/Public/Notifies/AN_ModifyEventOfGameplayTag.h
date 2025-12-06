// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "Components/TempestEventsComponent.h"
#include "AN_ModifyEventOfGameplayTag.generated.h"

/**
 *
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UAN_ModifyEventOfGameplayTag : public UAnimNotify
{
    GENERATED_BODY()

public:

    UAN_ModifyEventOfGameplayTag();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Base Variables")
    FGameplayTag EventToModify;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Base Variables")
    EEventModifyType EventModifyType;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
