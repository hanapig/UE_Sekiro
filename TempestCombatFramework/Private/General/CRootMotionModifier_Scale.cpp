// Fill out your copyright notice in the Description page of Project Settings.

#include "General/CRootMotionModifier_Scale.h"
#include "Components/TempestCombatComponent.h"
#include "GameFramework/Character.h"
#include "Objects/TempestAttackPropertiesObject.h"
#include "Objects/TempestAttackTraitObject.h"

UCRootMotionModifier_Scale::UCRootMotionModifier_Scale(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    //UpdatedScale = Scale * GetFloatPropertyValue();
}

float UCRootMotionModifier_Scale::GetFloatPropertyValue()
{
    if (UTempestCombatComponent* const CombatComponent = GetActorOwner()->FindComponentByClass<UTempestCombatComponent>())
    {
        if (CombatComponent->ReceivedAttackProperty != nullptr)
        {
            for (FInstancedAttackTrait& Elem : CombatComponent->ReceivedAttackProperty->AttackPropertyTraits)
            {
                if (UTempestPushMultiplier* const PushTrait = Cast<UTempestPushMultiplier>(Elem.AttackTraitProperty))
                {
                    return PushTrait->PushMultiplier;
                }
            }
        }
    }
    return 1.f;
}