// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Misc/Guid.h"
#include "ANS_UniqueGuidAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, HideDropdown)
class TEMPESTCORE_API UANS_UniqueGuidAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	FGuid AnimNotifyStateGuid;

	UFUNCTION(BlueprintCallable, Category ="Base Functions")
	void GenerateNewGuid();
	
};
