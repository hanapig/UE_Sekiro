// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Circle.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_Circle : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Circle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	
	UBehaviorTreeComponent* MyOwnerComp;

	UPROPERTY(EditAnywhere, Category="Strafe Settings")
	float Direction=0.0f;
	
	UPROPERTY(EditAnywhere, Category="Strafe Settings")
	float StrafeSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, Category="Strafe Settings")
	float StopTime = 3.f;

	float StartTime;
};
