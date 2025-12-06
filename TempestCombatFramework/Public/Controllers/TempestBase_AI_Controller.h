// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TempestBase_AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API ATempestBase_AI_Controller : public AAIController
{
	GENERATED_BODY()
public:
	ATempestBase_AI_Controller(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure,BlueprintCallable,Category = "Getters")
	void GetSightPerceptionViewPoint(FVector& out_Location, FRotator& out_Rotation) const;
	virtual void GetSightPerceptionViewPoint_Implementation(FVector& out_Location, FRotator& out_Rotation) const;
};
