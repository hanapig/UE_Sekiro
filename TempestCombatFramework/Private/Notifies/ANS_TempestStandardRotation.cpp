// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/ANS_TempestStandardRotation.h"


UANS_TempestStandardRotation::UANS_TempestStandardRotation()
{

}

void UANS_TempestStandardRotation::CreateRotationProperties(UObject* OuterToAssign)
{
	if (OuterToAssign == nullptr || CreatedRotationProperties.Num() > 0)
	{
		return;
	}

	if (!(RotationPropertiesToPerform.IsEmpty()))
	{
		for (FInstancedRotationProperty& Elem : RotationPropertiesToPerform)
		{
			FInstancedRotationProperty LocalInstancedRotationProperty;
			UTempestBaseRotationProperty* LocalNewRotation = DuplicateObject(Elem.RotationProperty, OuterToAssign);

			if (LocalNewRotation != nullptr)
			{
				LocalInstancedRotationProperty.RotationProperty = LocalNewRotation;
				LocalNewRotation->StartRotationProperty();
				CreatedRotationProperties.Emplace(LocalInstancedRotationProperty);
			}
		}
	}
	
}

void UANS_TempestStandardRotation::EndRotationProperties()
{
	if (!(CreatedRotationProperties.IsEmpty()))
	{
		for (FInstancedRotationProperty& Elem : CreatedRotationProperties)
		{
			if (Elem.RotationProperty != nullptr)
			{
				Elem.RotationProperty->EndRotationProperty();
			}
		}
		CreatedRotationProperties.Empty();
	}
}
