// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "Objects/TempestSpecialProperty.h"
#include "TempestPropertiesComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class TEMPESTCORE_API UTempestPropertiesComponent : public UTempestBaseComponent
{
	GENERATED_BODY()
public:
	UTempestPropertiesComponent();


	UPROPERTY()
	TArray<FInstancedSpecialProperties> SpecialProperties;

	UFUNCTION(BlueprintCallable , BlueprintPure,Category = "Base Functions")
	const TArray<FInstancedSpecialProperties>& GetSpecialProperties() const; 

	UFUNCTION(BlueprintCallable , Category = "Base Functions")
	void AddToSpecialProperties(UPARAM(ref) FInstancedSpecialProperties& SpecialPropertyToAdd);

	UFUNCTION(BlueprintCallable , Category = "Base Functions")
	void AddArrayToSpecialProperties(UPARAM(ref) TArray<FInstancedSpecialProperties>& SpecialPropertiesToAdd);

	UFUNCTION(BlueprintCallable , Category = "Base Functions")
	void ClearSpecialProperties();


	virtual void OnOwnerDestroyed(AActor* DestroyedActor) override;
};
