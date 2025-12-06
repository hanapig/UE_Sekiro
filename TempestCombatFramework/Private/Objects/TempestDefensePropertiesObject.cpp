// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 


#include "Objects/TempestDefensePropertiesObject.h"
#include "Objects/TempestBaseImpactEffect.h"
#include "GameFramework/Actor.h"

void UTempestDefensePropertiesObject::ConstructDefenseProperty_Implementation()
{

}

void UTempestDefensePropertiesObject::ProcessReceivedAttack_Implementation()
{

}

bool UTempestDefensePropertiesObject::GetCanInfluenceAttribute_Implementation(FGameplayTag AttributeToInfluence)
{
	return false;
}

float UTempestDefensePropertiesObject::InfluenceAttributeAmount_Implementation(FGameplayTag AttributeToInfluence, float AmountToInfluence)
{
	return 0.f;
}


void UTempestDefensePropertiesObject::ApplyHitEffectOfClass_Implementation(TSubclassOf<class UTempestBaseImpactEffect> ImpactEffectClass)
{
	if (ImpactEffectClass)
	{
		UTempestBaseImpactEffect* LocalFoundImpactEffect;
		GetHitEffectOfClass(ImpactEffectClass, LocalFoundImpactEffect);

		if (LocalFoundImpactEffect)
		{
			LocalFoundImpactEffect->ApplyImpactEffect();
		}
		else
		{
			LocalFoundImpactEffect = NewObject<UTempestBaseImpactEffect>(GetDefensePropertyOwner(), ImpactEffectClass);
			if (LocalFoundImpactEffect)
			{
				CreatedImpactEffects.AddUnique(LocalFoundImpactEffect);
				LocalFoundImpactEffect->ApplyImpactEffect();
			}
		}
	}
}

void UTempestDefensePropertiesObject::GetHitEffectOfClass_Implementation(TSubclassOf<class UTempestBaseImpactEffect> ImpactEffectClass, class UTempestBaseImpactEffect*& FoundImpactEffect)
{
	FoundImpactEffect = nullptr;
	if (CreatedImpactEffects.Num() > 0)
	{
		for (int32 i = 0; i < CreatedImpactEffects.Num(); i++)
		{
			if (CreatedImpactEffects[i])
			{
				if (CreatedImpactEffects[i]->GetClass() == ImpactEffectClass)
				{
					FoundImpactEffect = CreatedImpactEffects[i];
					return;
				}
			}
		}
	}
}
