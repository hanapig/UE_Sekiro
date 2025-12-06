// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 


#include "Components/TempestStatisticsComponent.h"
#include "Objects/TempestBaseStatisticObject.h"
#include "Components/TempestTickingComponent.h"

// Sets default values for this component's properties
UTempestStatisticsComponent::UTempestStatisticsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UTempestStatisticsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTempestStatisticsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTempestStatisticsComponent::OnOwnerDestroyed(AActor* DestroyedActor)
{
	ClearStatisticsComponent();
}

void UTempestStatisticsComponent::InitializeStatisticsComponent_Implementation()
{

}

void UTempestStatisticsComponent::ConstructStatisticOfClass(TSubclassOf< UTempestBaseStatisticObject> StatisticClassToCreate)
{
	if (StatisticClassToCreate)
	{
		UTempestBaseStatisticObject* LocalStatisticObject;
		LocalStatisticObject = NewObject<UTempestBaseStatisticObject>(GetOwner(), StatisticClassToCreate);

		if (LocalStatisticObject)
		{
			AvialableStatistics.AddUnique(LocalStatisticObject);
			LocalStatisticObject->ConstructStatistic();
		}
	}
}

void UTempestStatisticsComponent::GetStatisticOfGameplayTag(FGameplayTag StatisticGameplayTag, UTempestBaseStatisticObject*& FoundStatistic)
{
	FoundStatistic = nullptr;

	for (UTempestBaseStatisticObject*& StatisticElem : AvialableStatistics)
	{
		if (StatisticElem)
		{
			if (StatisticElem->StatisticGameplayTag == StatisticGameplayTag)
			{
				FoundStatistic = StatisticElem;
				return;
			}
		}
	}

	
}

void UTempestStatisticsComponent::GetStatisticOfClass(TSubclassOf< UTempestBaseStatisticObject> StatisticClass, UTempestBaseStatisticObject*& FoundStatistic)
{
	FoundStatistic = nullptr;
	for (UTempestBaseStatisticObject*& StatisticElem : AvialableStatistics)
	{
		if (StatisticElem)
		{
			if (StatisticElem->GetClass() == StatisticClass)
			{
				FoundStatistic = StatisticElem;
				return;
			}
		}
	}
}

void UTempestStatisticsComponent::ClearStatisticsComponent()
{
	for (int32 i = 0; i < AvialableStatistics.Num(); i++)
	{
		if (AvialableStatistics[i])
		{
			if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
			{
				TickingComponent->RemoveTickableObject(AvialableStatistics[i]);
			}
		}
	}

	AvialableStatistics.Empty();
}
