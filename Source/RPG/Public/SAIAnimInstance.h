// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SAIAnimInstance.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class RPG_API USAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	

	//速度控制
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	float SpeedContral;

	//角色指针
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	APawn*AIPawn;

	//获取速度
	UFUNCTION(BlueprintCallable,Category = "Return Speed")
	float GetMovementSpeed()const ;

	//返回角度
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	float CurrentYaw;

	//返回状态
	UPROPERTY(BlueprintReadWrite, Category = "Character State")
	float State;

	//返回带方向速度
	UPROPERTY(BlueprintReadWrite, Category = "SpeedDirection")
	float SpeedDirection;

	//控制前后方向状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MovementDirection=0.0f;

	//左右方向状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float leftdirection=0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LookAt")
	FVector LookAtLocation;
	
	virtual void NativeInitializeAnimation()override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	mutable UCharacterMovementComponent* MovementComponent;

};
