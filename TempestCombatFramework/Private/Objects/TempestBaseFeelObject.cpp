// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/TempestBaseFeelObject.h"
#include "Components/TempestFeelComponent.h"


UTempestBaseFeelObject::UTempestBaseFeelObject()
{

}

void UTempestBaseFeelObject::SetFeelOwner(AActor* NewFeelOwner)
{
	FeelOwner = NewFeelOwner;
}

void UTempestBaseFeelObject::ConstructFeelObject_Implementation()
{

}

void UTempestBaseFeelObject::ActivateFeelObject_Implementation()
{
	if (GetFeelOwner())
	{
		UTempestFeelComponent* LocalFeelComponent = GetFeelOwner()->FindComponentByClass<UTempestFeelComponent>();
		if (LocalFeelComponent)
		{
			FeelActivationStatus = EActivationStatus::Activated;
			LocalFeelComponent->OnFeelActivation.Broadcast(FeelGameplayTag);
			
		}
	}
}

void UTempestBaseFeelObject::DeactivateFeelObject_Implementation()
{
	if (GetFeelOwner())
	{
		UTempestFeelComponent* LocalFeelComponent = GetFeelOwner()->FindComponentByClass<UTempestFeelComponent>();
		if (LocalFeelComponent)
		{
			FeelActivationStatus = EActivationStatus::Deactivated;
			LocalFeelComponent->RemoveFromCreatedFeelProperties(this);
		}
	}
}

