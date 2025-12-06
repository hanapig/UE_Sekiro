// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Objects/TempestAttackPropertiesObject.h"
#include "ANS_AttackTrace.generated.h"

/**
 * 
 */
 class UTempestAttackPropertiesObject;





UCLASS(BlueprintType, Blueprintable, Abstract, HideDropdown)
class TEMPESTCOMBATFRAMEWORK_API UANS_AttackTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = "Attack Properties")
	FInstancedAttackProperties AssignedAttackProperty;

#if WITH_EDITOR
	virtual void ValidateAssociatedAssets() override;
#endif
};
