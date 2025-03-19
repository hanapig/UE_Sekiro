


#include "AI/BTTask_Circle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAIAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetMathLibrary.h"


UBTTask_Circle::UBTTask_Circle()
{
	bNotifyTick = true;  
	NodeName = TEXT("Task_Circle");
    StartTime= 0.0f;
    
}

EBTNodeResult::Type UBTTask_Circle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    MyOwnerComp = &OwnerComp;
    StartTime = 0.f;

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

    
    EnemyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    
    Direction = FMath::RandBool() ? 1.f : -1.f;
    
    
    return EBTNodeResult::InProgress;
}

void UBTTask_Circle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    StartTime += DeltaSeconds;

    AAIController* AICon = OwnerComp.GetAIOwner();

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    BlackboardComp->SetValueAsBool("Circleing", true);
    
    if (!AICon) 
    {
        FinishLatentTask(*MyOwnerComp, EBTNodeResult::Failed);
        return;
    }

    ACharacter* EnemyCharacter = Cast<ACharacter>(AICon->GetPawn());
    if (!EnemyCharacter)
    {
        FinishLatentTask(*MyOwnerComp, EBTNodeResult::Failed);
        return;
    }

    bool bCan_LeapAttack = BlackboardComp->GetValueAsBool("bCan_LeapAttack");
    
    if (bCan_LeapAttack)
    {
        FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    if (USkeletalMeshComponent* MeshComp = EnemyCharacter->GetMesh())
    {
        if (USAIAnimInstance* AIAnimInstance = Cast<USAIAnimInstance>(MeshComp->GetAnimInstance()))
        {
            AIAnimInstance->MovementDirection = Direction; 
        }
    }
    
    if (StartTime >= StopTime)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool("bHasCircled", true);

        USkeletalMeshComponent* MeshComp = EnemyCharacter->GetMesh();
        if (MeshComp)
        {
            USAIAnimInstance* AIAnimInstance = Cast<USAIAnimInstance>(MeshComp->GetAnimInstance());
            if (AIAnimInstance)
            {
                AIAnimInstance->MovementDirection = 0.f;
                AIAnimInstance->leftdirection = 0.f;
            }
        }
        
        FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
        return;
    }
    
    FVector PlayerLocation = BlackboardComp->GetValueAsVector("PlayerLocation");
    FVector EnemyLocation = EnemyCharacter->GetActorLocation();

    FVector ToPlayer = PlayerLocation - EnemyLocation;
    ToPlayer.Z = 0.0f;  
    float DistanceToPlayer = ToPlayer.Size();


    FVector DirToPlayer = ToPlayer.GetSafeNormal();

  
    float DesiredRadius = 700.f;  

   
    float DistDiff = DistanceToPlayer - DesiredRadius;
    

    FVector TangentDir = FVector::CrossProduct(DirToPlayer, FVector::UpVector).GetSafeNormal() * Direction;
    
    FVector MoveDir = TangentDir; 
    MoveDir = MoveDir.GetSafeNormal();
    EnemyCharacter->AddMovementInput(MoveDir, StrafeSpeed * DeltaSeconds);
   
    FRotator CurrentRotation = EnemyCharacter->GetActorRotation();
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, PlayerLocation);

 
    float RotationSpeed = 5.f;  
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed);
    EnemyCharacter->SetActorRotation(NewRotation);
    

    if (USkeletalMeshComponent* MeshComp = EnemyCharacter->GetMesh())
    {
        if (USAIAnimInstance* AIAnimInstance = Cast<USAIAnimInstance>(MeshComp->GetAnimInstance()))
        {
            AIAnimInstance->leftdirection = Direction; 
        }
    }
}