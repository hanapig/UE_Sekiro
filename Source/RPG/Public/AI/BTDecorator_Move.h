// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_Move.generated.h"


UCLASS()
class RPG_API UBTDecorator_Move : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

// public:
// 	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
// 	
// 	UPROPERTY(EditAnywhere, Category = "AI")
// 	float MinDistance = 300.f; 
//
// 	UPROPERTY(EditAnywhere, Category = "AI")
// 	float MaxDistance = 600.f;
//
// 	UPROPERTY(EditAnywhere, Category = "AI")
// 	float CurrentDistance = 400.f; 
// 	
};
