// Created By Aamn Chahrour & Samrudh Sunil. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/TempestBaseActor.h"
#include "Objects/TempestSpawnerProcessor.h"
#include "Tempest_AI_Spawner.generated.h"

/**
 * 
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API ATempest_AI_Spawner : public ATempestBaseActor
{
	GENERATED_BODY()
public:
	ATempest_AI_Spawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Initialization")
	FInstancedSpawnerProcessor SpawnProcessor;

	UFUNCTION(BlueprintCallable,Category ="Spawner Base Functions")
	void AssignProcessor();

#if WITH_EDITOR
	void OnObjectSelected(UObject* Object);
	bool bSelectedInEditor = true;
#endif
	
};
