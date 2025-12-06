// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "UObject/NoExportTypes.h"
#include "TempestBaseStateObject.generated.h"

/**
 * 
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseStateObject : public UTempestBaseObject
{
	GENERATED_BODY()

public:
	UTempestBaseStateObject();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State Properties")
	FGameplayTag StateGameplayTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State Properties")
	bool bTimeControlledState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State Controls", meta = (EditCondition = "bTimeControlledState"))
	float StateTimeLimit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State Controls", meta = (EditCondition = "bTimeControlledState"))
	float TimerUpdateRate = 0.1f;
	

protected:

	UPROPERTY(BlueprintReadWrite, Category = "State Time")
	float TimePassed;



	UPROPERTY()
	AActor* PerformingActor;

public:

				/* State Main Events */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Events")
	void ConstructState();
	virtual void ConstructState_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Events")
	void StartState();
	virtual void StartState_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Events")
	void EndState();
	virtual void EndState_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Events")
	void PreStateActivation();
	virtual void PreStateActivation_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Events")
	void PostStateActivation();
	virtual void PostStateActivation_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Events")
	void PreLossOfActiveState();
	virtual void PreLossOfActiveState_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Events")
	void PostLossOfActiveState();
	virtual void PostLossOfActiveState_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Events")
	bool RestartState(bool ConditionCheck);
	virtual bool RestartState_Implementation(bool ConditionCheck);


				/* State Main Functions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Functions")
	void ClearStateValues();
	virtual void ClearStateValues_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Functions")
	void StartStateTimer();
	virtual void StartStateTimer_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Functions")
	void IncrementPassedTime();
	virtual void IncrementPassedTime_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Functions")
	void ClearPassedTime();
	virtual void ClearPassedTime_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Functions")
	void StopStateTimer();
	virtual void StopStateTimer_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Functions")
	void OnRemovedState();
	virtual void OnRemovedState_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State Base Functions")
	float IncrementTimeByUpdateRate();
	virtual float IncrementTimeByUpdateRate_Implementation();


				/* State Setters */
	UFUNCTION(BlueprintCallable, Category = "State Setters")
	void SetPerformingActor(AActor* NewPerformingActor) { PerformingActor = NewPerformingActor; }

				/* State Getters */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Category = "State Getters")
	bool CanPerformState();
	virtual bool CanPerformState_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Category = "State Getters")
	bool CanSetAsQueueState();
	virtual bool CanSetAsQueueState_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State Getters")
	void GetPerformingActor(AActor*& CurrentPerformingActor) { CurrentPerformingActor = PerformingActor; }

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "State Getters")
	float GetTimePassed() { return TimePassed; }
};
