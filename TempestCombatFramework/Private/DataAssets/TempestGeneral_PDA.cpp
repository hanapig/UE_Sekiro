// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 


#include "DataAssets/TempestGeneral_PDA.h"
#include "Objects/TempestBaseAttributeObject.h"
#include "Objects/TempestBaseStateObject.h"

TArray<UTempestBaseAttributeObject*> UTempestGeneral_PDA::GetAttributesToCreate() const
{
	TArray<UTempestBaseAttributeObject*> LocalArray;
	if (AttributesToCreate.IsEmpty())
	{
		return LocalArray;
	}

	for (int32 i = 0; i < AttributesToCreate.Num(); i++)
	{
		if (AttributesToCreate[i].AttributeToCreate)
		{
			LocalArray.Emplace(AttributesToCreate[i].AttributeToCreate);
		}
	}

	return LocalArray;
}
