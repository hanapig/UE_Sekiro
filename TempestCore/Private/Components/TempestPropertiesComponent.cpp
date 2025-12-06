// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TempestPropertiesComponent.h"
#include "Components/TempestTickingComponent.h"

UTempestPropertiesComponent::UTempestPropertiesComponent()
{

}

const TArray<FInstancedSpecialProperties>& UTempestPropertiesComponent::GetSpecialProperties() const
{
	return SpecialProperties;
}

void UTempestPropertiesComponent::AddToSpecialProperties(FInstancedSpecialProperties& PropertyToAdd)
{
	if (PropertyToAdd.SpecialProperty)
	{
		if (GetOwner())
		{
			FInstancedSpecialProperties LocalSpecialProperty;
			FInstancedSpecialProperties LocalProperty;
			UTempestSpecialProperty* LocalNewSpecialProperty = DuplicateObject(PropertyToAdd.SpecialProperty, GetOwner());
			if (LocalNewSpecialProperty)
			{
				LocalSpecialProperty.SpecialProperty = LocalNewSpecialProperty;
				SpecialProperties.Emplace(LocalSpecialProperty);
				LocalNewSpecialProperty->OnPropertyCreated(GetOwner());
			}
		}
	}
}

void UTempestPropertiesComponent::AddArrayToSpecialProperties(TArray<FInstancedSpecialProperties>& SpecialPropertiesToAdd)
{
	if (SpecialPropertiesToAdd.IsEmpty())
	{
		return;
	}

	for (int32 i = 0; i < SpecialPropertiesToAdd.Num(); i++)
	{
		AddToSpecialProperties(SpecialPropertiesToAdd[i]);
	}
}

void UTempestPropertiesComponent::ClearSpecialProperties()
{
	for (FInstancedSpecialProperties& Elem : SpecialProperties)
	{
		if (Elem.SpecialProperty)
		{
			if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
			{
				TickingComponent->RemoveTickableObject(Elem.SpecialProperty);
			}
		}
	}

	SpecialProperties.Empty();
}

void UTempestPropertiesComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
	ClearSpecialProperties();
}
