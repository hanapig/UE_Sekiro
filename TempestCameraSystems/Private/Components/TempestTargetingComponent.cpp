// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 


#include "Components/TempestTargetingComponent.h"
#include "Components/TempestTargetSceneComponent.h"
#include "Components/TempestTickingComponent.h"

// Sets default values for this component's properties
UTempestTargetingComponent::UTempestTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UTempestTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (DefaultTargetingObject)
	{
		UTempestBaseTargetingObject* LocalTargetingObject;
		ConstructTargetingObjectOfClass(DefaultTargetingObject, LocalTargetingObject);
		if (LocalTargetingObject)
		{
			SetCurrentActiveTargetingObject(LocalTargetingObject);
		}
	}
}


// Called every frame
void UTempestTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}


void UTempestTargetingComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
	ClearTargetingComponent();
}

void UTempestTargetingComponent::GetTargetingObjectOfGameplayTag(FGameplayTag TargetingObjectTag, UTempestBaseTargetingObject*& FoundTargetingObject)
{
	FoundTargetingObject = nullptr;
	for (int32 i = 0; i < TargetingObjects.Num(); i++)
	{
		if (TargetingObjects[i])
		{
			if (TargetingObjects[i]->TargetingObjectGameplayTag == TargetingObjectTag)
			{
				FoundTargetingObject = TargetingObjects[i];
				return;
			}
		}
	}
	return;
}

bool UTempestTargetingComponent::GetIsCurrentelyTargeting()
{
	if (CurrentActiveTargetingObject)
	{
		UTempestTargetSceneComponent* LocalTargetComponent;
		CurrentActiveTargetingObject->GetTargetedComponent(LocalTargetComponent);
		if (LocalTargetComponent)
		{
			return true;
		}
	}
	return false;
}

void UTempestTargetingComponent::ConstructTargetingObjectOfClass(TSubclassOf<UTempestBaseTargetingObject> TargetingObjectToConstruct, UTempestBaseTargetingObject*& ConstructedTargetingObject)
{
	ConstructedTargetingObject = nullptr;
	if (TargetingObjectToConstruct)
	{
		UTempestBaseTargetingObject* LocalNewTargetingObject;
		LocalNewTargetingObject = NewObject<UTempestBaseTargetingObject>(GetOwner(), TargetingObjectToConstruct);

		if (LocalNewTargetingObject)
		{
			TargetingObjects.AddUnique(LocalNewTargetingObject);
			LocalNewTargetingObject->ConstructTargetingObject();
			ConstructedTargetingObject = LocalNewTargetingObject;
		}
		
	}
}

void UTempestTargetingComponent::SetCurrentActiveTargetingObject(UTempestBaseTargetingObject* TargetingObject)
{
	CurrentActiveTargetingObject = TargetingObject;
	OnActiveTargetingObjectChanged.Broadcast(CurrentActiveTargetingObject);
}

void UTempestTargetingComponent::RemoveFromTargetingObjects(UTempestBaseTargetingObject* TargetingObjectToRemove)
{
	if (TargetingObjectToRemove)
	{
		TargetingObjects.Remove(TargetingObjectToRemove);
		if (CurrentActiveTargetingObject == TargetingObjectToRemove)
		{
			CurrentActiveTargetingObject = nullptr;
		}
	}
}



void UTempestTargetingComponent::ClearTargetingComponent()
{
	for (int32 i = 0; i < TargetingObjects.Num(); i++)
	{
		if (TargetingObjects[i])
		{
			if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
			{
				TickingComponent->RemoveTickableObject(TargetingObjects[i]);
			}
		}
	}

	TargetingObjects.Empty();
}

AActor* UTempestTargetingComponent::GetTargetedActor()
{
	if (CurrentActiveTargetingObject)
	{
		UTempestTargetSceneComponent* LocalTargetComponent;
		CurrentActiveTargetingObject->GetTargetedComponent(LocalTargetComponent);
		if (LocalTargetComponent)
		{
			return LocalTargetComponent->GetOwner();
		}
	}
	return nullptr;
}

