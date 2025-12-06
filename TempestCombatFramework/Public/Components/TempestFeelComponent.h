// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "GameplayTagContainer.h"
#include "Objects/TempestBaseFeelObject.h"
#include "TempestFeelComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFeelObjectActivation, FGameplayTag, FeelGameplayTag);

UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestFeelComponent : public UTempestBaseComponent
{
	GENERATED_BODY()
public:
	UTempestFeelComponent();

	virtual void OnOwnerDestroyed(AActor* DestroyedActor) override;

    UPROPERTY(BlueprintAssignable, BlueprintCallable,Category="Your Category")
	FFeelObjectActivation OnFeelActivation;

	UPROPERTY()
	TArray<UTempestBaseFeelObject*> CreatedFeelProperties;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category ="Getters")
	void GetFeelObjectOfGameplayTag(FGameplayTag FeelGameplayTag, UTempestBaseFeelObject*& FoundFeelObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category ="Getters")
	void GetCreatedFeelProperties(TArray<UTempestBaseFeelObject*>& FoundCreatedFeelProperties);

	UFUNCTION(BlueprintCallable, Category ="Process")
	void AddToCreatedFeelProperties(FInstancedFeelProperties InstancedFeelProperty, UTempestBaseFeelObject*& FeelCreated);

	UFUNCTION(BlueprintCallable, Category ="Process")
	void RemoveFromCreatedFeelProperties(UTempestBaseFeelObject* FeelToRemove);

	UFUNCTION(BlueprintCallable, Category ="Process")
	void ClearFeelProperties();
	
};
