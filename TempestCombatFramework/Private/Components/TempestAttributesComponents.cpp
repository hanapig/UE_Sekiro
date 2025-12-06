// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 


#include "Components/TempestAttributesComponents.h"
#include "Objects/TempestBaseAttributeObject.h"
#include "Components/TempestTickingComponent.h"

// Sets default values for this component's properties
UTempestAttributesComponents::UTempestAttributesComponents()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UTempestAttributesComponents::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTempestAttributesComponents::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

}


void UTempestAttributesComponents::OnOwnerDestroyed(AActor* DestroyedActor)
{
	ClearAttributes();
	ClearAttributeModifiers();
}

void UTempestAttributesComponents::AssignAttributes(TArray<UTempestBaseAttributeObject*> AttributesToAssign)
{
	for (int32 i = 0; i < AttributesToAssign.Num(); i++)
	{
		if (AttributesToAssign[i])
		{

			UTempestBaseAttributeObject* LocalNewAttribute = DuplicateObject(AttributesToAssign[i],GetOwner());

			LocalNewAttribute->SetOwningActor(GetOwner());
			LocalNewAttribute->ConstructAttribute();

			CreatedAttributes.AddUnique(LocalNewAttribute);
		}
	}
}

void UTempestAttributesComponents::ClearAttributes()
{
	for (int32 i = 0; i < CreatedAttributes.Num(); i++)
	{
		if (CreatedAttributes[i])
		{
			if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
			{
				TickingComponent->RemoveTickableObject(CreatedAttributes[i]);
			}
		}
	}

	CreatedAttributes.Empty();
}

void UTempestAttributesComponents::ClearAttributeModifiers()
{
	for (int32 i = 0; i < CreatedAttributeModifiers.Num(); i++)
	{
		if (CreatedAttributeModifiers[i])
		{
			if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
			{
				TickingComponent->RemoveTickableObject(CreatedAttributeModifiers[i]);
			}
		}
	}
	CreatedAttributeModifiers.Empty();
}

void UTempestAttributesComponents::ConstructAttributeOfClass(TSubclassOf<UTempestBaseAttributeObject> AttributeToConstruct, UTempestBaseAttributeObject*& ConstructedAttribute)
{
	ConstructedAttribute = nullptr;
	if (AttributeToConstruct)
	{
		UTempestBaseAttributeObject* LocalNewAttribute;
		LocalNewAttribute = NewObject<UTempestBaseAttributeObject>(GetOwner(), AttributeToConstruct);

		CreatedAttributes.AddUnique(LocalNewAttribute);

		LocalNewAttribute->SetOwningActor(GetOwner());
		LocalNewAttribute->ConstructAttribute();
		ConstructedAttribute = LocalNewAttribute;
	}

}

void UTempestAttributesComponents::GetAttributeOfClass(TSubclassOf<UTempestBaseAttributeObject> AttributeToSearch, UTempestBaseAttributeObject*& FoundAttribute)
{
	FoundAttribute = nullptr;
	for (int32 i = 0; i < CreatedAttributes.Num(); i++)
	{

		if (CreatedAttributes[i])
		{
			if (CreatedAttributes[i]->GetClass() == AttributeToSearch)
			{
				FoundAttribute = CreatedAttributes[i];

				return;
			}
		}

	}

	FoundAttribute = nullptr;
	return;
}

void UTempestAttributesComponents::GetAttributeModifierOfClass(TSubclassOf<UTempestBaseAttributeModifier> AttributeModifierToSearch, UTempestBaseAttributeModifier*& FoundAttributeModifier)
{
	FoundAttributeModifier = nullptr;
	for (int32 i = 0; i < CreatedAttributeModifiers.Num(); i++)
	{

		if (CreatedAttributeModifiers[i])
		{
			if (CreatedAttributeModifiers[i]->GetClass() == AttributeModifierToSearch)
			{
				FoundAttributeModifier = CreatedAttributeModifiers[i];

				return;
			}
		}

	}

	FoundAttributeModifier = nullptr;
	return;
}

void UTempestAttributesComponents::GetAttributeModifierOfGamplayTag(FGameplayTag AttributeModifierTagToSearch, UTempestBaseAttributeModifier*& FoundAttributeModifier)
{
	FoundAttributeModifier = nullptr;
	for (int32 i = 0; i < CreatedAttributeModifiers.Num(); i++)
	{

		if (CreatedAttributeModifiers[i])
		{
			if (CreatedAttributeModifiers[i]->AttributeModifierTag == AttributeModifierTagToSearch)
			{
				FoundAttributeModifier = CreatedAttributeModifiers[i];
				return;
			}
		}

	}

	FoundAttributeModifier = nullptr;
	return;
}

UTempestBaseAttributeObject*& UTempestAttributesComponents::GetAttributeOfGameplayTag(FGameplayTag AttributeGameplayTag)
{
	for (int32 i = 0; i < CreatedAttributes.Num(); i++)
	{
		if (CreatedAttributes[i])
		{
			if (CreatedAttributes[i]->AttributeGameplayTag == AttributeGameplayTag)
			{
				return CreatedAttributes[i];
				
			}
		}
	}

	static UTempestBaseAttributeObject* LocalAttribute = nullptr;
	return LocalAttribute;
}

void UTempestAttributesComponents::RemoveFromAttributeModifiers(UTempestBaseAttributeModifier* AttributeModifierToRemove)
{
	if (AttributeModifierToRemove)
	{
		if (UTempestTickingComponent* TickingComponent = GetOwner()->FindComponentByClass<UTempestTickingComponent>())
		{
			TickingComponent->RemoveTickableObject(AttributeModifierToRemove);
		}
		CreatedAttributeModifiers.Remove(AttributeModifierToRemove);
	}
}

void UTempestAttributesComponents::AddToAttributeModifiers(UTempestBaseAttributeModifier* AttributeModifierToAdd)
{
	if (AttributeModifierToAdd)
	{
		CreatedAttributeModifiers.Emplace(AttributeModifierToAdd);
	}
}

void UTempestAttributesComponents::ConstructAttributeModifierOfClass(TSubclassOf<UTempestBaseAttributeModifier> AttributeModifierToConstruct,UTempestBaseAttributeModifier*& ConstructedAttributeModifier)
{
	ConstructedAttributeModifier = nullptr;

	if (AttributeModifierToConstruct)
	{
		UTempestBaseAttributeModifier* LocalNewAttributeModifier;
		LocalNewAttributeModifier = NewObject<UTempestBaseAttributeModifier>(GetOwner(), AttributeModifierToConstruct);
		LocalNewAttributeModifier->ConstructAttributeModifier();
		ConstructedAttributeModifier = LocalNewAttributeModifier;
	}
}

