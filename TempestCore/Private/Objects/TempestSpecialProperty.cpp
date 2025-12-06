// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved. 


#include "Objects/TempestSpecialProperty.h"

UTempestSpecialProperty::UTempestSpecialProperty()
{

}

void UTempestSpecialProperty::OnPropertyCreated_Implementation(UObject* NewPropertyOwner)
{
	if (NewPropertyOwner)
	{
		PropertyOwner = NewPropertyOwner;
	}
}

void UTempestSpecialProperty::SetPropertyOwner(UObject* NewOwner)
{
	PropertyOwner = NewOwner;
}

bool UTempestSpecialProperty::GetCanProcessProperty_Implementation()
{
	return true;
}

void UTempestSpecialProperty::ProcessSpecialProperty_Implementation()
{

}
