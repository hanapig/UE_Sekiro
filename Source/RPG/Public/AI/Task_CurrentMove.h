// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Animation/AnimInstance.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_CurrentMove.generated.h"

UCLASS()
class RPG_API UTask_CurrentMove : public UBTTaskNode
{
	GENERATED_BODY()
public:
	
	UTask_CurrentMove();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaintainDistance")
	float MinDistance = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaintainDistance")
	float MaxDistance = 800.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaintainDistance")
	float AcceptanceRadius = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaintainDistance")
	float MovementDirectionValue = 0.0f;
	
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	UBehaviorTreeComponent* MyOwnerComp = nullptr;
};
