// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "UObject/NoExportTypes.h"
#include "TempestBaseStatisticObject.generated.h"

/**
 * 
 */
UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseStatisticObject : public UTempestBaseObject
{
	GENERATED_BODY()
public:
	UTempestBaseStatisticObject();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Statistic Properties")
	FGameplayTag StatisticGameplayTag;

	UPROPERTY(BlueprintReadWrite, Category = "Statistic Properties")
	float StatisticCurrentValue = 0.f;


				/* Statistic Base Functions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Statistic Base Functions")
	void ConstructStatistic();
	virtual void ConstructStatistic_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Statistic Base Functions")
	void UpdateStatisticValueBy(float NewStatisticValue);
	virtual void UpdateStatisticValueBy_Implementation(float NewStatisticValue);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Statistic Base Functions")
	void ResetStatisticValue();
	virtual void ResetStatisticValue_Implementation();
};
