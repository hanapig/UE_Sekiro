// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Objects/TempestBaseAttributeObject.h"
#include "TempestGeneral_PDA.generated.h"

/**
 * 
 */



UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestGeneral_PDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Attributes")
	TArray<FInstancedAttributes> AttributesToCreate;


	UFUNCTION(BlueprintCallable, Category ="DA Base functions")
	TArray<UTempestBaseAttributeObject*> GetAttributesToCreate() const;


};
