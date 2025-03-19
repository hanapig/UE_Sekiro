// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheackStiffTag.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTDecorator_CheackStiffTag : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheackStiffTag();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	
	FGameplayTag StiffTag; 
};
