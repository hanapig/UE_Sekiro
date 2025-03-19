// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_MoveSpeed.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "SAICharacter.h"

UBTTaskNode_MoveSpeed::UBTTaskNode_MoveSpeed()
{
	
	NodeName = "Set Speed";

	
	NewSpeed = 400.f;
}

EBTNodeResult::Type UBTTaskNode_MoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	if (AIController)
	{
		APawn* ControlledPawn = AIController->GetPawn();

		ACharacter* SAICharacter = Cast<ASAICharacter>(ControlledPawn);
		
		if (ControlledPawn)
		{
			
			UCharacterMovementComponent* MovementComponent = SAICharacter->GetCharacterMovement();
			if (MovementComponent)
			{
				
				MovementComponent->MaxWalkSpeed = NewSpeed;
			}
		}
	}

	
	return EBTNodeResult::Succeeded;
}