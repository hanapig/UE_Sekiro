// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestAttributesComponents.h"
#include "TempestTenacityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTenacityDifferenceChanged, float, Difference);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CCZCLIENT_API UTempestTenacityComponent : public UTempestAttributesComponents
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTempestTenacityComponent();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float TenacityCompareDifference = 0.0f;

	UPROPERTY(BlueprintAssignable, Category = "Tenacity")
	FOnTenacityDifferenceChanged OnTenacityDifferenceChanged;
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Tenacity")
	float CompareTenacityDifference(AActor* Other, AActor*& OutSmallerActor);
	virtual float CompareTenacityDifference_Implementation(AActor* Other, AActor*& OutSmallerActor);
	
	UFUNCTION(BlueprintPure, Category = "Tenacity")
	float GetCurrentTenacityValue() const;

	UFUNCTION(BlueprintCallable, Category = "Tenacity")
	void SetCurrentTenacityValue(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Tenacity")
	void ModifyCurrentTenacityValue(float ModifyBy);
	
	
};
