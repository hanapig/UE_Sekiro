// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SetAllowedInputs.generated.h"

/**
 *
 */


class UTempestBaseObject;


UCLASS(BlueprintType, Blueprintable, Abstract, HideDropdown)
class TEMPESTCOMBATFRAMEWORK_API UANS_SetAllowedInputs : public UAnimNotifyState
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buffer Type")
	TArray<class UInputAction*> AllowedInputs;


private:
	UPROPERTY()
	UTempestBaseObject* BufferingObject = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = "Buffer Type")
	void SetBufferingObject(UTempestBaseObject* NewBufferingObject){BufferingObject = NewBufferingObject;};

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Buffer Type")
	UTempestBaseObject* GetBufferingObject(){return BufferingObject;}
};
