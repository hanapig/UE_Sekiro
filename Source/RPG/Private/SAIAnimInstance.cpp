// Fill out your copyright notice in the Description page of Project Settings.


#include "SAIAnimInstance.h"

#include "SAICharacter.h"

float USAIAnimInstance::GetMovementSpeed() const
{
	if (MovementComponent)
	{
		return MovementComponent->Velocity.Size();

	}
	return 0.0f;
}



//动画实例
void USAIAnimInstance::NativeInitializeAnimation()
{
	
	if (!AIPawn)
	{
		AIPawn = TryGetPawnOwner();
	}

	if (AIPawn)
	{
		AIPawn = TryGetPawnOwner();
		ACharacter* SAICharacter = Cast<ASAICharacter>(AIPawn);
		if (SAICharacter)
		{
			MovementComponent = SAICharacter->GetCharacterMovement();
		}
	}
	
}

void USAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	
	APawn* OwningPawn = TryGetPawnOwner();
	if (!OwningPawn) return;


	ASAICharacter* AIChar = Cast<ASAICharacter>(OwningPawn);
	if (AIChar)
	{
		LookAtLocation = AIChar->PlayerHeadLocation;
	}
}