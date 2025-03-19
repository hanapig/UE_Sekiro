// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Stop.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_Stop : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	// 重写执行任务
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// 控制是否已触发通知
	bool bNotifyTriggered = false;

public:
	
	void OnNotifyTriggered();
};
