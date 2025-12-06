// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#include "Objects/TempestBaseBuffObject.h"
#include "Components/TempestCombatComponent.h"

UTempestBaseBuffObject::UTempestBaseBuffObject()
{
}

void UTempestBaseBuffObject::SetDamageDealer(AActor* NewDamageDealer)
{
    DamageDealers.Emplace(NewDamageDealer);
}

void UTempestBaseBuffObject::SetDamageReceiver(AActor* NewDamageReceiver)
{
    DamageReceiver = NewDamageReceiver;
}

void UTempestBaseBuffObject::StartBuff_Implementation()
{
}

void UTempestBaseBuffObject::ReapplyBuff_Implementation()
{
    if (DamageReceiver != nullptr)
    {
        if (UTempestCombatComponent* const CombatComponent = DamageReceiver->FindComponentByClass<UTempestCombatComponent>())
        {
            CombatComponent->OnOwnerBuffReapplied.Broadcast(this);
        }
    }
}

void UTempestBaseBuffObject::EndBuff_Implementation()
{
    if (DamageReceiver != nullptr)
    {
        if (UTempestCombatComponent* const CombatComponent = DamageReceiver->FindComponentByClass<UTempestCombatComponent>())
        {
            CombatComponent->RemoveFromAppliedBuffs(this);
        }
    }
}

bool UTempestBaseBuffObject::GetCanApplyBuff_Implementation()
{
    return true;
}