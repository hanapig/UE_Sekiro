
#include "SAIController.h"

#include "SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	APawn*MyPawn=UGameplayStatics::GetPlayerPawn(this,0);

	ASAICharacter* EnemyCharacter = Cast<ASAICharacter>(GetPawn());

	
	
	if (MyPawn)
	{
		 GetBlackboardComponent()->SetValueAsVector("Move",MyPawn->GetActorLocation());
		
		GetBlackboardComponent()->SetValueAsObject("Pawn",MyPawn);
	}
	
}

void ASAIController::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); 
	ASAICharacter* EnemyCharacter = Cast<ASAICharacter>(GetPawn());

	
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	
	if (PlayerCharacter && EnemyCharacter)
	{
		
		GetBlackboardComponent()->SetValueAsVector("PlayerLocation", PlayerCharacter->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector("EnemyLocation", EnemyCharacter->GetActorLocation());
		
	}
}





