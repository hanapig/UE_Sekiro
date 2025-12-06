// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#include "Objects/TempestAttackPropertiesObject.h"
#include "Objects/TempestBaseBuffObject.h"
#include <Engine/EngineTypes.h>
#include "GameFramework/Actor.h"

UTempestAttackPropertiesObject::UTempestAttackPropertiesObject()
{
}

AActor* UTempestAttackPropertiesObject::GetAttackPropertyOwner()
{
    return AttackPropertyOwner;
}

void UTempestAttackPropertiesObject::SetAttackPropertyOwner(AActor* NewAttacker)
{
    AttackPropertyOwner = NewAttacker;
}

AActor* UTempestAttackPropertiesObject::GetAttackPropertyInstigator()
{
    return AttackPropertyInstigator;
}

void UTempestAttackPropertiesObject::SetAttackPropertyInstigator(AActor* NewAttackerInstigator)
{
    AttackPropertyInstigator = NewAttackerInstigator;
}

class UTempestBaseTraceObject* UTempestAttackPropertiesObject::GetHitAttackTraceOwner()
{
    return HitTraceObject;
}

void UTempestAttackPropertiesObject::SetHitAttackTrace(class UTempestBaseTraceObject* InHitTrace)
{
    HitTraceObject = InHitTrace;
}

void UTempestAttackPropertiesObject::SetHitResult(FHitResult NewHitResult)
{
    HitResult = NewHitResult;
    OnHitResultUpdated.Broadcast(HitResult);
}

void UTempestAttackPropertiesObject::SetImpactResult(FGameplayTag NewImpactResult)
{
    ImpactResult = NewImpactResult;
}

void UTempestAttackPropertiesObject::AssignImpactProperties()
{
    TArray<FInstancedImpactProperties> LocalImpactProperties;

    if (AttackPropertyOwner)
    {
        for (FInstancedImpactProperties Elem : ImpactProperties)
        {
            UTempestBaseImpactObject* LocalNewImpactProperty;
            if (Elem.ImpactProperty)
            {
                LocalNewImpactProperty = nullptr;
                LocalNewImpactProperty = DuplicateObject(Elem.ImpactProperty, AttackPropertyOwner);

                if (LocalNewImpactProperty)
                {
                    LocalNewImpactProperty->SetAttackProperty(this);
                    FInstancedImpactProperties LocalValue;
                    LocalValue.ImpactProperty = LocalNewImpactProperty;

                    LocalImpactProperties.Emplace(LocalValue);
                }
            }
        }
        ImpactProperties = LocalImpactProperties;
    }
}

void UTempestAttackPropertiesObject::ApplyImpactProperties_Implementation()
{
}

void UTempestAttackPropertiesObject::ConstructAttackProperty_Implementation()
{
    TArray<FInstancedAttackTrait> TraitsArray;
    for (int32 i = 0; i < AttackPropertyTraits.Num(); i++)
    {
        FInstancedAttackTrait AttackTrait;
        if (AttackPropertyTraits[i].AttackTraitProperty && AttackPropertyOwner)
        {
            UTempestAttackTraitObject* LocalNewTrait = DuplicateObject(AttackPropertyTraits[i].AttackTraitProperty, AttackPropertyOwner);
            if (LocalNewTrait)
            {
                AttackTrait.AttackTraitProperty = LocalNewTrait;
                AttackTrait.AttackTraitProperty->AttackProperty = this;
                TraitsArray.Emplace(AttackTrait);
            }
        }
    }

    AttackPropertyTraits = TraitsArray;
}

void UTempestAttackPropertiesObject::ProcessAttackProperty_Implementation()
{
}

void UTempestAttackPropertiesObject::ProcessAttackTraits_Implementation()
{
    for (int32 i = 0; i < AttackPropertyTraits.Num(); i++)
    {
        if (AttackPropertyTraits[i].AttackTraitProperty != nullptr)
        {
            AttackPropertyTraits[i].AttackTraitProperty->ProcessAttackTrait();
        }
    }
}

void UTempestAttackPropertiesObject::GetImpactPropertyOfGameplayTag(FInstancedImpactProperties& PropertyFound, FGameplayTag PropertyToFind)
{
    for (FInstancedImpactProperties Elem : ImpactProperties)
    {
        if (Elem.ImpactProperty)
        {
            if (Elem.ImpactProperty->ImpactPropertyTag == PropertyToFind)
            {
                PropertyFound = Elem;
                return;
            }
        }
    }
}

UTempestAttackTraitObject* UTempestAttackPropertiesObject::GetAttackTraitOfGameplayTag(FGameplayTag InGameplayTag)
{
    if (InGameplayTag.IsValid())
    {
        for (int32 i = 0; i < AttackPropertyTraits.Num(); i++)
        {
            if (AttackPropertyTraits[i].AttackTraitProperty != nullptr &&
                AttackPropertyTraits[i].AttackTraitProperty->TraitTag == InGameplayTag)
            {
                return AttackPropertyTraits[i].AttackTraitProperty;
            }
        }
    }

    return nullptr;
}