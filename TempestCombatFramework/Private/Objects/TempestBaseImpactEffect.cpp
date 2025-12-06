// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/TempestBaseImpactEffect.h"


void UTempestBaseImpactEffect::ApplyImpactEffect_Implementation()
{

}

FInstancedImpactEffectProperties UTempestBaseImpactEffect::ConstructImpactEffectProperty(FInstancedImpactEffectProperties PropertyToConstruct)
{
    FInstancedImpactEffectProperties LocalImpactEffectProperty;
    if (PropertyToConstruct.ImpactEffectProperty)
    {
        if (GetOuter())
        {
            UTempestImpactEffectProperty* LocalNewImpactEffectProperty = DuplicateObject(PropertyToConstruct.ImpactEffectProperty, GetOuter());
            if (LocalNewImpactEffectProperty)
            {
                LocalImpactEffectProperty.ImpactEffectProperty = LocalNewImpactEffectProperty;
            }
        }

    }
    return LocalImpactEffectProperty;
}

