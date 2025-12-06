// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TempestFeelComponent.h"
#include "Components/TempestTickingComponent.h"

UTempestFeelComponent::UTempestFeelComponent()
{

}

void UTempestFeelComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
	ClearFeelProperties();
}

void UTempestFeelComponent::GetFeelObjectOfGameplayTag(FGameplayTag FeelGameplayTag, UTempestBaseFeelObject*& FoundFeelObject)
{
	if (CreatedFeelProperties.Num() > 0)
	{
		for (int32 i = 0; i < CreatedFeelProperties.Num(); i++)
		{
			if (CreatedFeelProperties[i])
			{
				if (CreatedFeelProperties[i]->FeelGameplayTag == FeelGameplayTag)
				{
					FoundFeelObject = CreatedFeelProperties[i];
					break;
				}
			}
		}
	}
}

void UTempestFeelComponent::GetCreatedFeelProperties(TArray<UTempestBaseFeelObject*>& FoundCreatedFeelProperties)
{
	FoundCreatedFeelProperties = CreatedFeelProperties;
}

void UTempestFeelComponent::AddToCreatedFeelProperties(FInstancedFeelProperties InstancedFeelProperty, UTempestBaseFeelObject*& FeelCreated)
{
	if (InstancedFeelProperty.FeelObject)
	{
		UTempestBaseFeelObject* LocalBaseFeelObject = DuplicateObject(InstancedFeelProperty.FeelObject, GetOwner());
		if (LocalBaseFeelObject)
		{
			LocalBaseFeelObject->SetFeelOwner(GetOwner());
			CreatedFeelProperties.Emplace(LocalBaseFeelObject);
			FeelCreated = LocalBaseFeelObject;
		}
	}
}

void UTempestFeelComponent::RemoveFromCreatedFeelProperties(UTempestBaseFeelObject* FeelToRemove)
{
	if (FeelToRemove)
	{
		if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
		{
			TickingComponent->RemoveTickableObject(FeelToRemove);
		}
		CreatedFeelProperties.Remove(FeelToRemove);
	}
}

void UTempestFeelComponent::ClearFeelProperties()
{
	if (CreatedFeelProperties.Num() > 0)
	{
		for (int32 i = 0; i < CreatedFeelProperties.Num(); i++)
		{
			if (CreatedFeelProperties[i])
			{
				if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
				{
					TickingComponent->RemoveTickableObject(CreatedFeelProperties[i]);
				}
			}
		}
	}

	CreatedFeelProperties.Empty();
}
