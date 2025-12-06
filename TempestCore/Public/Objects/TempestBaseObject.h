// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Interfaces/TempestTickingInterface.h"
#include "TempestBaseObject.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, BlueprintType)
class TEMPESTCORE_API UTempestBaseObject : public UObject, public ITempestTickingInterface
{
	GENERATED_BODY()

public:
	//To Set The World in the Object to be able to use some BP nodes
	virtual UWorld* GetWorld() const override;

};
