// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "TempestHitFeedBackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CCZCLIENT_API UTempestHitFeedBackComponent : public UTempestBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTempestHitFeedBackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FTimerHandle HitReactionTimerHandle;
	
	UFUNCTION(BlueprintCallable, Category = "Hit Reaction")
	void FrameRate(float DelayTime = 0.0f);
};
