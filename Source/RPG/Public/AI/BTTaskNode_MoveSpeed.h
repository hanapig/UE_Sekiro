// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MoveSpeed.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTaskNode_MoveSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	
	UBTTaskNode_MoveSpeed();

protected:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float NewSpeed;

	
};
