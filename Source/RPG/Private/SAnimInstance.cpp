// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"

//返回角色移动速度
float USAnimInstance::GetMovementSpeed() const
{
	if (MovementComponent)
	{
		return MovementComponent->Velocity.Size();

	}
	return 0.0f;
}



//动画实例
void USAnimInstance::NativeInitializeAnimation()
{

	if (!MyPawn)
	{
		MyPawn = TryGetPawnOwner();
	}

	if (MyPawn)
	{
		MyPawn = TryGetPawnOwner();
		ACharacter* SCharacter = Cast<ACharacter>(MyPawn);
		if (SCharacter)
		{
			MovementComponent = SCharacter->GetCharacterMovement();
		}
	}
}

void USAnimInstance::SetRootMotionFromEverything()
{
	
	// SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
	// ACharacter* MyCharacter = Cast<ACharacter>(TryGetPawnOwner());
	// if (MyCharacter)
	// {
	// 	MyCharacter->GetCharacterMovement()->StopMovementImmediately();
	// }
}

void USAnimInstance::SetRootMotionFromMontagesOnly()
{
	
	// SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	// ACharacter* MyCharacter = Cast<ACharacter>(TryGetPawnOwner());
	// if (MyCharacter)
	// {
	// 	MyCharacter->GetCharacterMovement()->StopMovementImmediately();
	// }
}