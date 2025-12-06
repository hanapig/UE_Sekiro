// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved. 


#include "Objects/TempestBaseObject.h"
#include "Engine/World.h"
#include "Components/TempestTickingComponent.h"

UWorld* UTempestBaseObject::GetWorld() const
{
	// To Make sure the outer is Valid and can be used
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		//Attempt to get the world 
		AActor* Outer = GetTypedOuter<AActor>();
		if (Outer != nullptr)
		{
			return Outer->GetWorld();
		}
	}
	return nullptr;
}
