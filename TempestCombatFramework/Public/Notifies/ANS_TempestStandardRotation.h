// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Objects/TempestBaseRotationProperty.h"
#include "ANS_TempestStandardRotation.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, HideDropdown)
class TEMPESTCOMBATFRAMEWORK_API UANS_TempestStandardRotation : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UANS_TempestStandardRotation();


	UPROPERTY(EditAnywhere, Category="Base Variables")
	TArray<FInstancedRotationProperty> RotationPropertiesToPerform;

	UPROPERTY(BlueprintReadOnly, Category="Base Variables")
	TArray<FInstancedRotationProperty> CreatedRotationProperties;

	UFUNCTION(BlueprintCallable, Category = "Base Functions")
	void CreateRotationProperties(UObject* OuterToAssign);

	UFUNCTION(BlueprintCallable, Category = "Base Functions")
	void EndRotationProperties();


};
