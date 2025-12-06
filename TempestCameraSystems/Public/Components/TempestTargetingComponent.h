// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "GameplayTagContainer.h"
#include "Objects/TempestBaseTargetingObject.h"
#include "TempestTargetingComponent.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTempestBaseTargetingObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveTargetingObjectChanged, UTempestBaseTargetingObject*, NewTargetingObject);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class TEMPESTCAMERASYSTEMS_API UTempestTargetingComponent : public UTempestBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTempestTargetingComponent();

	UPROPERTY(BlueprintAssignable, BlueprintCallable,Category = "Dispatchers")
	FOnActiveTargetingObjectChanged OnActiveTargetingObjectChanged;

	UPROPERTY(VisibleAnywhere, Category = "Base Variables")
	TArray<UTempestBaseTargetingObject*> TargetingObjects;

	UPROPERTY(VisibleAnywhere, Category = "Base Variables")
	UTempestBaseTargetingObject* CurrentActiveTargetingObject;

	UPROPERTY(EditAnywhere, Category = "Base Variables")
	TSubclassOf<UTempestBaseTargetingObject> DefaultTargetingObject;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnOwnerDestroyed(AActor* DestroyedActor) override;

	UFUNCTION(BlueprintCallable, BlueprintPure ,Category = "Targeting Component Getters")
	FORCEINLINE UTempestBaseTargetingObject*& GetCurrentActiveTargetingObject() {return CurrentActiveTargetingObject;}

	UFUNCTION(BlueprintCallable, BlueprintPure ,Category = "Targeting Component Getters")
	FORCEINLINE TArray<UTempestBaseTargetingObject*>& GetTargetingObjects(){ return TargetingObjects; }

	UFUNCTION(BlueprintCallable, BlueprintPure ,Category = "Targeting Component Getters")
	void GetTargetingObjectOfGameplayTag(FGameplayTag TargetingObjectTag, UTempestBaseTargetingObject*& FoundTargetingObject);

	UFUNCTION(BlueprintCallable, BlueprintPure ,Category = "Targeting Component Getters")
	bool GetIsCurrentelyTargeting();

	UFUNCTION(BlueprintCallable ,Category = "Targeting Component Constrction")
	void ConstructTargetingObjectOfClass(TSubclassOf<UTempestBaseTargetingObject> TargetingObjectToConstruct, UTempestBaseTargetingObject*& ConstructedTargetingObject);

	UFUNCTION(BlueprintCallable ,Category = "Targeting Component Setters")
	void SetCurrentActiveTargetingObject(UTempestBaseTargetingObject* TargetingObject);

	UFUNCTION(BlueprintCallable ,Category = "Targeting Component Setters")
	void RemoveFromTargetingObjects(UTempestBaseTargetingObject* TargetingObjectToRemove);

	UFUNCTION(BlueprintCallable ,Category = "Targeting Component Setters")
	void ClearTargetingComponent();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Targeting Component Getters")
	AActor* GetTargetedActor();


};
