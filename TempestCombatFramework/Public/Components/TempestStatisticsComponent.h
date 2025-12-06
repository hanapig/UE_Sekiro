// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "GameplayTagContainer.h"
#include "TempestStatisticsComponent.generated.h"

class UTempestBaseStatisticObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TEMPESTCOMBATFRAMEWORK_API UTempestStatisticsComponent : public UTempestBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTempestStatisticsComponent();

	UPROPERTY(BlueprintReadWrite, Category = "Statistics Base Info")
	TArray<UTempestBaseStatisticObject*> AvialableStatistics;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnOwnerDestroyed(AActor* DestroyedActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Statistics Base Functions")
	void InitializeStatisticsComponent();
	virtual void InitializeStatisticsComponent_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Statistics Base Functions")
	void ConstructStatisticOfClass(TSubclassOf< UTempestBaseStatisticObject> StatisticClassToCreate);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Statistics Base Functions")
	void GetStatisticOfGameplayTag(FGameplayTag StatisticGameplayTag, UTempestBaseStatisticObject*& FoundStatistic);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Statistics Base Functions")
	void GetStatisticOfClass(TSubclassOf< UTempestBaseStatisticObject> StatisticClass, UTempestBaseStatisticObject*& FoundStatistic);
		
	UFUNCTION(BlueprintCallable, Category = "Statistics Base Functions")
	void ClearStatisticsComponent();
};
