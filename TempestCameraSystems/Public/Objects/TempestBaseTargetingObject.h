// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "GameplayTagContainer.h"
#include "Components/TempestTargetSceneComponent.h"
#include "TempestBaseTargetingObject.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedTargetedComponent);

UCLASS()
class TEMPESTCAMERASYSTEMS_API UTempestBaseTargetingObject : public UTempestBaseObject
{
	GENERATED_BODY()
public:
	UTempestBaseTargetingObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	FGameplayTag TargetingObjectGameplayTag;

	UPROPERTY()
	UTempestTargetSceneComponent* TargetedComponent;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
	FOnUpdatedTargetedComponent OnUpdatedTargetedComponent;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
	void ConstructTargetingObject();
	virtual void ConstructTargetingObject_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
	void ToggleTargeting(bool ToggleValue);
	virtual void ToggleTargeting_Implementation(bool ToggleValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
	void UpdateTargeting();
	virtual void UpdateTargeting_Implementation();


	/* Setters & Getters */
	UFUNCTION(BlueprintCallable, Category = "Setters")
	void SetTargetedComponent(UTempestTargetSceneComponent* NewTargetedComponent);

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Getters")
	void GetTargetedComponent(UTempestTargetSceneComponent*& CurrentTargetingComponent){CurrentTargetingComponent = TargetedComponent;}
	
};
