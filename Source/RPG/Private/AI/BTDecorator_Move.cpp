


#include "AI/BTDecorator_Move.h"

#include "BehaviorTree/BlackboardComponent.h"

// bool UBTDecorator_Move::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
// {
//
// 	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
// 	
// 	if (!BlackboardComp) return false;
//
// 	FVector PlayerLocation = BlackboardComp->GetValueAsVector("PlayerLocation");
// 	FVector EnemyLocation = BlackboardComp->GetValueAsVector("EnemyLocation");
// 	
// 	float Distance = FVector::Dist(PlayerLocation, EnemyLocation);
//
//
// 	if (Distance < MinDistance)
// 	{
// 		FVector Direction = (EnemyLocation - PlayerLocation).GetSafeNormal(); 
// 		FVector NewLocation = EnemyLocation + Direction * (MinDistance - Distance); 
// 		BlackboardComp->SetValueAsVector("MoveLocation", NewLocation);
// 		return true;
// 	}
//
//
// 	if (Distance >MaxDistance)
// 	{
//
// 		FVector Direction = (PlayerLocation - EnemyLocation).GetSafeNormal(); 
// 		FVector NewLocation = EnemyLocation + Direction * (Distance - MaxDistance);  
// 		BlackboardComp->SetValueAsVector("MoveLocation", NewLocation);
// 		return true;
// 	}
// 	
//
// 	return false;
// }