// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheackStiffTag.h"
#include "SCharacter.h"
#include "SAICharacter.h"
#include "SAIController.h"
#include "Kismet/GameplayStatics.h"

UBTDecorator_CheackStiffTag::UBTDecorator_CheackStiffTag()
{
	NodeName = "CheackStiffTag";
}


bool UBTDecorator_CheackStiffTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		
		ASCharacter* PlayerCharacter = Cast<ASCharacter>(UGameplayStatics::GetActorOfClass(AIController->GetWorld(), ASCharacter::StaticClass()));
		if (PlayerCharacter)
		{
			
			if (PlayerCharacter->BlockTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Stiff"))))
			{
				return true;  
			}
		}
	}

	return false;  
}