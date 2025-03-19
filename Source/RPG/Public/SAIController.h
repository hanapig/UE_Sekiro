// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "SAIController.generated.h"

UCLASS()
class RPG_API ASAIController : public AAIController
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	
protected:

	UPROPERTY(EditAnywhere,Category="AI");
	UBehaviorTree* BehaviorTree;

	
	
};
