// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/TempestAttackTraitObject.h"

UTempestAttackTraitObject::UTempestAttackTraitObject()
    : Super()
    , TraitTag()
    , AttackProperty(nullptr)
{
}

void UTempestAttackTraitObject::ProcessAttackTrait_Implementation()
{
}

UTempestPushMultiplier::UTempestPushMultiplier()
    : Super()
    , PushMultiplier(1.f)
{
}