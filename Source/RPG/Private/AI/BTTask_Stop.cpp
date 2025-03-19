// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Stop.h"
#include "AIController.h"
#include "SAICharacter.h"

EBTNodeResult::Type UBTTask_Stop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	if (bNotifyTriggered)
	{
		AAIController* AICon = OwnerComp.GetAIOwner();
		
		ASAICharacter* EnemyCharacter = Cast<ASAICharacter>(AICon->GetPawn());
		
		EnemyCharacter->StopAttack();
		
		return EBTNodeResult::Succeeded;
	}
	
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Stop::OnNotifyTriggered() 
{
	
	bNotifyTriggered = true;
}