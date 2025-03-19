// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:


	//速度控制
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float SpeedContral;

	//角色指针
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	APawn*MyPawn;

	//获取速度
	UFUNCTION(BlueprintCallable,Category = "Return Speed")
	float GetMovementSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Root Motion")
	void SetRootMotionFromEverything();

	// 设置根运动模式为RootMotionFromMontagesOnly
	UFUNCTION(BlueprintCallable, Category = "Root Motion")
	void SetRootMotionFromMontagesOnly();

	//返回角度
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	float CurrentYaw;

	//返回状态
	UPROPERTY(BlueprintReadWrite, Category = "Character State")
	float State;

	//返回带方向速度
	UPROPERTY(BlueprintReadWrite, Category = "SpeedDirection")
	float SpeedDirection;

	bool IsStart=true;
	
	bool bHasStarted = false;

	virtual void NativeInitializeAnimation()override;

private:
	mutable UCharacterMovementComponent* MovementComponent;

};
