// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "Objects/TempestImpactEffectProperty.h"
#include "TempestBaseImpactEffect.generated.h"

/**
 * 
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseImpactEffect : public UTempestBaseObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
	void ApplyImpactEffect();
	virtual void ApplyImpactEffect_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Base Functions")
	FInstancedImpactEffectProperties ConstructImpactEffectProperty(FInstancedImpactEffectProperties PropertyToConstruct);

};
