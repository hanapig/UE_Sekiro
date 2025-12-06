// Created By Aamn Chahrour & Samrudh Sunil. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_RotateToInputDirectionCPP.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, HideDropdown)
class TEMPESTCOMBATFRAMEWORK_API UANS_RotateToInputDirectionCPP : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Base Variables")
	FVector InputDirection;


	UFUNCTION(BlueprintCallable, Category = "Base Functions")
	void SetInputDirection(FVector NewInputDirection) { InputDirection = NewInputDirection;}
	
};
