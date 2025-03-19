
#include "AI/Task_CurrentMove.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAIAnimInstance.h"
#include "SAICharacter.h"
#include "Animation/AnimInstance.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UTask_CurrentMove::UTask_CurrentMove()
{
	
	bNotifyTick = true;

	NodeName = TEXT("Task_CurrentMove");
    
}

EBTNodeResult::Type UTask_CurrentMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    EnemyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    

    if (!EnemyCharacter)
    {
        return EBTNodeResult::Failed;
    }
    
    const FVector PlayerLocation = BlackboardComp->GetValueAsVector("PlayerLocation");
    const FVector EnemyLocation  = EnemyCharacter->GetActorLocation();
    
    const float Distance  = FVector::Dist(EnemyLocation, PlayerLocation);
    
    
    FVector MoveLocation;
    MovementDirectionValue = 0.f;

    if (Distance < MinDistance)
    {
        
        MovementDirectionValue = -1.f;

        EnemyCharacter->LaunchCharacter(FVector(-120,0,0), false, false);
        
        FVector Direction = (EnemyLocation - PlayerLocation).GetSafeNormal();
        MoveLocation = EnemyLocation + Direction * MinDistance;
        
    }
    else if (Distance > MaxDistance)
    {
     
        MovementDirectionValue = 1.f;

        EnemyCharacter->LaunchCharacter(FVector(120,0,0), false, false);
        
        FVector Direction = (PlayerLocation - EnemyLocation).GetSafeNormal();
        MoveLocation = EnemyLocation + Direction * MaxDistance;
        
    }
    else
    {
       
        MovementDirectionValue = 0.f;
       
        USkeletalMeshComponent* MeshComp = EnemyCharacter->GetMesh();
        if (MeshComp)
        {
            USAIAnimInstance* AIAnimInstance = Cast<USAIAnimInstance>(MeshComp->GetAnimInstance());
            if (AIAnimInstance)
            {
                AIAnimInstance->MovementDirection = 0.f;
            }
        }
        return EBTNodeResult::Succeeded;
    }
    
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector("MoveLocation", MoveLocation);
    }
   
    if (USkeletalMeshComponent* MeshComp = EnemyCharacter->GetMesh())
    {
        if (USAIAnimInstance* AIAnimInstance = Cast<USAIAnimInstance>(MeshComp->GetAnimInstance()))
        {
            AIAnimInstance->MovementDirection = MovementDirectionValue;
        }
    }

  
    if (UCharacterMovementComponent* MovementComp = EnemyCharacter->GetCharacterMovement())
    {
        MovementComp->MaxWalkSpeed = 80.f; 
    }

  
    EPathFollowingRequestResult::Type MoveResult = AICon->MoveToLocation(MoveLocation, AcceptanceRadius, true);

    

    if (MoveResult == EPathFollowingRequestResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

 
    AICon->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UTask_CurrentMove::OnMoveCompleted);
    
    return EBTNodeResult::InProgress;

    
}

void UTask_CurrentMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    AAIController* AICon = OwnerComp.GetAIOwner();
    ACharacter* EnemyCharacter = Cast<ACharacter>(AICon->GetPawn());
    const FVector PlayerLocation = BlackboardComp->GetValueAsVector("PlayerLocation");
    const FVector EnemyLocation  = EnemyCharacter->GetActorLocation();
    
    const float Distance  = FVector::Dist(EnemyLocation, PlayerLocation);

    BlackboardComp->SetValueAsFloat("DistanceToPlayer", Distance);
    
    
}

void UTask_CurrentMove::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    
    if (!MyOwnerComp)
    {
        return; 
    }

   
    EBTNodeResult::Type TaskResult = EBTNodeResult::Failed;

    if (Result.Code == EPathFollowingResult::Success)
    {
     
        TaskResult = EBTNodeResult::Succeeded;
    }
    else
    {
      
        TaskResult = EBTNodeResult::Failed;
    }

    FinishLatentTask(*MyOwnerComp, TaskResult);
}