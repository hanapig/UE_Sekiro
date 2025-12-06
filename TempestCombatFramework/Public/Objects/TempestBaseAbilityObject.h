// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "TempestBaseAbilityObject.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Abilities & Tags"))
struct FAbilitiesAndGameplayTags
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Variables")
	FGameplayTag AbilitiesTag = FGameplayTag::RequestGameplayTag(FName("List.One"));

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Variables")
	TArray <TSubclassOf<class UTempestBaseAbilityObject>> Abilities;

	FAbilitiesAndGameplayTags() {};
};


USTRUCT(BlueprintType, meta = (DisplayName = "Abilities & Tags Array"))
struct FAbilitiesAndGameplayTagsArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Variables")
	TArray < FAbilitiesAndGameplayTags> AbilitiesAndGameplayTagsArray;

	FAbilitiesAndGameplayTagsArray() {};
};

UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseAbilityObject : public UTempestBaseObject
{
	GENERATED_BODY()
public:
	UTempestBaseAbilityObject();




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	FGameplayTag AbilityGameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	bool bHasCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Controls", meta = (EditCondition = "bHasCooldown"))
	float CooldownDuration = 0.f;


protected:
	UPROPERTY(BlueprintReadWrite, Category = "Ability Properties")
	class UAnimMontage* ActiveAbilityMontage;

	UPROPERTY()
	AActor* PerformingActor;

	UPROPERTY(BlueprintReadWrite, Category = "Cooldown")
	bool bAbilityOnCooldown;

public:
			/* Ability Main Functions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void ConstructAbility();
	virtual void ConstructAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void StartAbility();
	virtual void StartAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void EndAbility();
	virtual void EndAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void PreAbilityActivation();
	virtual void PreAbilityActivation_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void PostAbilityActivation();
	virtual void PostAbilityActivation_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void PreLossOfActiveAbility();
	virtual void PreLossOfActiveAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void PostLossOfActiveAbility();
	virtual void PostLossOfActiveAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void InterruptAbility();
	virtual void InterruptAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void CancelAbility();
	virtual void CancelAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void OnRemovedAbility();
	virtual void OnRemovedAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void ApplyAbilityCooldown();
	virtual void ApplyAbilityCooldown_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability Base Functions")
	void CooldownReset();
	virtual void CooldownReset_Implementation();

			/* Ability Getters */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category = "Ability Getters")
	bool CanPerformAbility();
	virtual bool CanPerformAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category = "Ability Getters")
	bool CanRePerformAbility();
	virtual bool CanRePerformAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category = "Ability Getters")
	bool CanIntteruptAbility();
	virtual bool CanIntteruptAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category = "Ability Getters")
	bool CanAbilityBeCanceled();
	virtual bool CanAbilityBeCanceled_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category = "Ability Getters")
	bool GetIsAbilityOnCooldown();
	virtual bool GetIsAbilityOnCooldown_Implementation();

	

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability Getters")
	void GetPerformingActor(AActor*& CurrentPerformingActor) { CurrentPerformingActor = PerformingActor; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability Getters")
	void GetActiveAbilityMontage(UAnimMontage*& CurrentActiveMontage) { CurrentActiveMontage = ActiveAbilityMontage; }


				/* Ability Setters */
	UFUNCTION(BlueprintCallable, Category = "Ability Setters")
	void SetActiveAbilityMontage(UAnimMontage* NewActiveMontage) { ActiveAbilityMontage = NewActiveMontage; }

	UFUNCTION(BlueprintCallable, Category = "Ability Setters")
	void SetPerformingActor(AActor* NewPerformingActor){PerformingActor = NewPerformingActor;}
};
