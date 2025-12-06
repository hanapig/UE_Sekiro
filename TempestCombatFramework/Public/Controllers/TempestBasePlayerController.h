// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TempestBasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API ATempestBasePlayerController : public APlayerController
{
	GENERATED_BODY()

//Functions	
public:
	/** Constructor */
	ATempestBasePlayerController();


	//~ Begin AController Interface
	virtual void OnPossess(class APawn* InPawn) override;
	//~ End AController Interface


//Variables
public:
	UPROPERTY(BlueprintReadOnly, Category ="Tempest Debug Info")
	class UTempestDebugObject* TempestDebugObject;

};
