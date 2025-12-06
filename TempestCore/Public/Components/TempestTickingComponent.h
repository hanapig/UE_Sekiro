// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "TempestTickingComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class TEMPESTCORE_API UTempestTickingComponent : public UTempestBaseComponent
{
	GENERATED_BODY()

public:
	UTempestTickingComponent();

	UPROPERTY()
	TArray <UObject*> TickableObjects;

	UFUNCTION(BlueprintCallable, Category ="Tempest Ticking Component | Process")
	void AddTickableObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category ="Tempest Ticking Component | Process")
	void RemoveTickableObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category ="Tempest Ticking Component | Getters")
	void GetTickableObjects(TArray <UObject*>& FoundTickableObjects);


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
