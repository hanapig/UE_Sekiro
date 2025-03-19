// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MotionWarpingComponent.h>

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "SAICharacter.generated.h"

class ASCharacter;

UCLASS()
class RPG_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

	bool bCanwished=false;

	//防止重复反击
	float ReSpecial=3.0f;

	bool bIsInStaggerState = false;
	int32 StaggerHitCount = 0;
	
protected:

	virtual void BeginPlay() override;

	FVector InitialLocation;
	FRotator InitialRotation;
	FVector OriginalLocation;
	FVector WarpLocation;

	FVector ArcCenter;

	float SmoothRadiusTime = 0.5f;
	const float MinimumRadius = 500.0f;
	float Radius = 600.0f;

	float MoveElapsedTime = 0.0f;
	float MoveDuration = 1.f; 

	bool bIsArcMoving = false;

	bool bIsRightArc;
	
	FTimerHandle SetStop;
	FTimerHandle ReSpecialTime;
	FTimerHandle MoveLeftTimerHandle;
	FTimerHandle MoveTimerHandle;

	bool bTaskFinished = false;

	bool bBlockedTriggered=false;

	int ParryCount = 0;
	
	float Speed;
	float MoveTime;
	float ElapsedTime;
	FVector StartLocation;
	bool bIsMovingRight;
	bool bIsMovingForward;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AttackMontageNotify(FName NotifyName,const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_NotifySuccess(FName NotifyName,const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_Punch(FName NotifyName,const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_Blocked_l(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_Blocked_l2(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_UP(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void SetCanwishedStart(FName NotifyName, const FBranchingPointNotifyPayload& Branch);
	
	UFUNCTION()
	void SetCanwishedStop(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void StopCollision(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Startretreating(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Stopretreating(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void StartParry_Animontion(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void BCanCombo(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_Right_Run(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_ForWard_Run(FName NotifyName, const FBranchingPointNotifyPayload& Branch);
	
	UFUNCTION(BlueprintCallable)
	void OnParried();

	UFUNCTION()
	void OnParried_Spike();
	
	UFUNCTION()
	void OnParried_L();

	UFUNCTION()
	void OnAI_AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Notify_Blocked_Special(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_Left(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_Right(FName NotifyName, const FBranchingPointNotifyPayload& Branch);
	
	UFUNCTION()
	void StartCombo(FName NotifyName, const FBranchingPointNotifyPayload& Branch);
	
	UFUNCTION()
	void ComboStill(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void ComboSArch(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Spike_Block_Combo(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Combo3(FName NotifyName, const FBranchingPointNotifyPayload& Branch);
	

	UFUNCTION()
	void Combo4(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Combo5(FName NotifyName, const FBranchingPointNotifyPayload& Branch);
	
	
	// UFUNCTION()
	// void ParryCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AI_Montage_Attack1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AI_Montage_Attack2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AI_Montage_Attack3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AI_Montage_Parry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AI_Montage_Parry_Spike;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AI_Montage_Parry_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* InjuredMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Blocked_l;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Blocked_l2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Blocked_l3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Blocked_Special;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Blocked_Combo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Blocked_Combo2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Blocked_Combo3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Blocked_Combo4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Blocked_Combo5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* AIMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* InjurySound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Slate")
	bool bIsBlock;

	UPROPERTY(EditDefaultsOnly, Category = "Slate")
	bool bIsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	ASCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI")
	FVector PlayerHeadLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI")
	FVector PlayerFootLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action1")
	UMotionWarpingComponent* AIMotionWarpings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer AttackTags;

	//后坐力相关1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsLeftMoving = false;
	
	//后坐力相关2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector TargetLocation;
	
	//后坐力相关3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float InterpSpeed = 5.0f;

	//后坐力相关3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float L_InterpSpeed = 3.0f; 

	
	void StartAttack();

	void StopAttack();

	void StartParry();
	
	void StopParry();

	void AI_Attack(AActor*AI_Actor,const FVector& AI_Location);

	void OnInjured();

	void StartRecoilCooldown();

	void StopMoveLeft();

	void ComboStill();

	void PlayAnimMontage_StillCombo();
	
};

