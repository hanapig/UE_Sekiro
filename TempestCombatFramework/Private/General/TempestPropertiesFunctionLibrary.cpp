// Fill out your copyright notice in the Description page of Project Settings.

#include "General/TempestPropertiesFunctionLibrary.h"
#include "Objects/TempestAttackPropertiesObject.h"

FInstanced_AI_BehaviorProperty UTempestPropertiesFunctionLibrary::CreateDuplicateBehaviorProperty(UObject* Owner, FInstanced_AI_BehaviorProperty BehaviorPropertyToDuplicate)
{
    FInstanced_AI_BehaviorProperty BehaviorProperty;
    if (BehaviorPropertyToDuplicate.AI_BehaviorToCreate && Owner)
    {
        UTempestBase_AI_BehaviorObject* LocalNewObject = DuplicateObject(BehaviorPropertyToDuplicate.AI_BehaviorToCreate, Owner);
        if (LocalNewObject)
        {
            BehaviorProperty.AI_BehaviorToCreate = LocalNewObject;
        }
    }
    return BehaviorProperty;
}

FInstancedAttackProperties UTempestPropertiesFunctionLibrary::CreateDuplicateAttackProperty(UObject* Owner, FInstancedAttackProperties AttackPropertyToDuplicate)
{
    FInstancedAttackProperties AttackProperty;
    AActor* const OwnerActor = Cast<AActor>(Owner);

    if (AttackPropertyToDuplicate.AttackProperty != nullptr && OwnerActor != nullptr)
    {
        UTempestAttackPropertiesObject* LocalNewAttackProperty = DuplicateObject(AttackPropertyToDuplicate.AttackProperty, Owner);
        AttackProperty.AttackProperty = LocalNewAttackProperty;
        LocalNewAttackProperty->SetAttackPropertyOwner(OwnerActor);
        LocalNewAttackProperty->SetAttackPropertyInstigator(OwnerActor);
        LocalNewAttackProperty->SetHitAttackTrace(nullptr);
        LocalNewAttackProperty->ConstructAttackProperty();
    }

    return AttackProperty;
}