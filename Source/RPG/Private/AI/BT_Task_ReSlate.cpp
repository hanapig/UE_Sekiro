#include "AI/BT_Task_ReSlate.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAICharacter.h"
#include "Animation/AnimInstance.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBT_Task_ReSlate::UBT_Task_ReSlate()
{
    NodeName = TEXT("Task_ReSlate");
    bNotifyTick = true;
}

EBTNodeResult::Type UBT_Task_ReSlate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    MyOwnerComp = &OwnerComp;
    
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    ACharacter* EnemyCharacter = Cast<ACharacter>(AICon->GetPawn());
    if (!EnemyCharacter)
    {
        return EBTNodeResult::Failed;
    }

    // 获取玩家角色，并计算玩家正前方的位置
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter)
    {
        return EBTNodeResult::Failed;
    }

    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector PlayerForward = PlayerCharacter->GetActorForwardVector();
    float OffsetDistance = 0.f; 
    FVector TargetLocation = PlayerLocation + PlayerForward * OffsetDistance;
    
 
    BlackboardComp->SetValueAsVector(TEXT("MoveLocation"), TargetLocation);

 
    BlackboardComp->SetValueAsBool(TEXT("IsMove"), true);
    
    
 
    if (UCharacterMovementComponent* MovementComp = EnemyCharacter->GetCharacterMovement())
    {
        MovementComp->MaxWalkSpeed = 600.f; 
    }
    
  
    EPathFollowingRequestResult::Type MoveResult = AICon->MoveToLocation(TargetLocation, AcceptableRadius, true, true, true);
    if (MoveResult == EPathFollowingRequestResult::Failed)
    {
    
        BlackboardComp->SetValueAsBool(TEXT("IsMove"), false);
        return EBTNodeResult::Failed;
    }
    
 
    AICon->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBT_Task_ReSlate::OnMoveCompleted);
    
    return EBTNodeResult::InProgress;
}


void UBT_Task_ReSlate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    AAIController* AICon = OwnerComp.GetAIOwner();
    ACharacter* EnemyCharacter = Cast<ACharacter>(AICon->GetPawn());
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    
    if (EnemyCharacter && PlayerCharacter)
    {
        float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
        BlackboardComp->SetValueAsFloat(TEXT("DistanceToPlayer"), Distance);
        
        // 如果距离小于一定阈值，认为已到达目标
        if (Distance <= AcceptableRadius)
        {
            BlackboardComp->SetValueAsBool(TEXT("IsMove"), false);
            BlackboardComp->SetValueAsBool(TEXT("MoveComplet"), true);
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    }
}



void UBT_Task_ReSlate::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    if (!MyOwnerComp)
    {
        return;
    }
    
 
    UBlackboardComponent* BlackboardComp = MyOwnerComp->GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsBool(TEXT("IsMove"), false);
        BlackboardComp->SetValueAsBool(TEXT("MoveComplet"), true);
    }
    
    EBTNodeResult::Type TaskResult = (Result.Code == EPathFollowingResult::Success) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
    FinishLatentTask(*MyOwnerComp, TaskResult);
}
