// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "Objects/TempestBaseAbilityObject.h"
#include "GameplayTagContainer.h"
#include "TempestBaseAbilityManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedCurrentActiveAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityActivated, UTempestBaseAbilityObject*, ActivatedAbility);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseAbilityManagerComponent : public UTempestBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTempestBaseAbilityManagerComponent();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
	FOnUpdatedCurrentActiveAbility OnUpdatedCurrentActiveAbility;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
	FOnAbilityActivated OnAbilityActivation;

	UPROPERTY(VisibleAnywhere, Category = "Base Variables")
	UTempestBaseAbilityObject* CurrentActiveAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Base Variables")
	TArray<UTempestBaseAbilityObject*> PassiveAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Base Variables")
	TArray<UTempestBaseAbilityObject*> ActivatableAbilities;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnOwnerDestroyed(AActor* DestroyedActor) override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Initialization Functions")
	void InitializeAbilitySystemComponent();
	virtual void InitializeAbilitySystemComponent_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Performing Ability")
	void PerformAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSet);
	
	UFUNCTION(BlueprintCallable, Category = "Performing Ability")
	bool TryPerformAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSet, bool ConditionCheck = true);

	UFUNCTION(BlueprintCallable, Category = "Performing Ability")
	bool TryPerformAbilitiesOfClass(TArray <TSubclassOf<UTempestBaseAbilityObject>> AbilitiesToSet, bool ConditionCheck = true);

	UFUNCTION(BlueprintCallable, Category = "Performing Ability")
	void SetCurrentActiveAbility(UTempestBaseAbilityObject* NewCurrentActiveAbility);

	UFUNCTION(BlueprintCallable, Category = "Performing Ability")
	void SetAsPassiveAbility(UTempestBaseAbilityObject* NewPassiveAbility);

	UFUNCTION(BlueprintCallable, Category = "Performing Ability")
	void RemoveFromPassiveAbilities(UTempestBaseAbilityObject* AbilityToRemove);

	UFUNCTION(BlueprintCallable, Category = "Setters")
	void RemoveFromActivatableAbilities(UTempestBaseAbilityObject* AbilityToRemove);



	/* Getters */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	UTempestBaseAbilityObject*& GetCurrentActiveAbility();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	void GetAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSearch, UTempestBaseAbilityObject*& FoundAbility);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	bool GetCanPerformAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSearch);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	void GetPassiveAbilities(TArray<UTempestBaseAbilityObject*>& PassiveAbilitiesArray) { PassiveAbilitiesArray = PassiveAbilities; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	void GetActivatableAbilities(TArray<UTempestBaseAbilityObject*>& ActivatableAbilitiesArray) { ActivatableAbilitiesArray = ActivatableAbilities; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	void GetChildAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToSearch, UTempestBaseAbilityObject*& FoundAbility);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	UTempestBaseAbilityObject*& GetAbilityOfGameplayTag(FGameplayTag AbilityGameplayTag);



	/* Construction */
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void ConstructAbilityOfClass(TSubclassOf<UTempestBaseAbilityObject> AbilityToConstruct, UTempestBaseAbilityObject*& ConstructedAbility);
		
	/* Clears All The Arrays Associated For Finding and Setting Abilities*/
	UFUNCTION(BlueprintCallable, Category = "Clear")
	void ClearAbilityComponent();
};
