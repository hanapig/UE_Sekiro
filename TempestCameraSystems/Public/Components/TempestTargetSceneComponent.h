// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TempestTargetSceneComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToggleFocus, bool, bCurrentlyFocused);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPESTCAMERASYSTEMS_API UTempestTargetSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTempestTargetSceneComponent();


    UPROPERTY(BlueprintAssignable, BlueprintCallable ,category = "Base Functions")
	FToggleFocus OnFocusToggled;
		
};
