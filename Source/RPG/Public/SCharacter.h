// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SAICharacter.h"
#include "SCharacter.generated.h"

class UMotionWarpingComponent;

UCLASS()
class RPG_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//弹簧臂组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	//摄像机组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	//存武器指针
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	//存武器实例
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon;


	ASCharacter();

protected:
	virtual void BeginPlay() override;

	//上一帧速度
	FVector LastVelocity;

	//蒙太奇动画
	FName NextName;
	
	FName BlockName;

	bool bIsRunning;

	bool bIsAttack = true;

	bool bIsTurn = false;

	bool bCanAttack = false; 

	bool bIsCombo = true;

	bool bIsLockOn=false;

	bool bIsBlock1Active = true;
	
	bool PlayerbIsMoving = false;

	bool bCanPlayAnim = true;

	bool spikes = false;

	bool bIsRetreating;

	float AttackTime = 2.0f;

	FVector CapsuleLocation;
	
	FVector PlayerTargetLocation;
	
	float PlayerInterpSpeed = 5.0f; 

	float LastAttackInputTime = 0.f; 
	const float AttackInputDebounce = 0.05f;
	
	int32 CurrentAttackStage;

	int32 BlockAnimIndex = 0;

	int AttackCount = 0;

	float LastClickTime = 0.0f;
	const float Clicklimit = 0.3f;

	float RetreatSpeed=200.0f;

	FTimerHandle AttackTimer;
	FTimerHandle ComboTimer;
	FTimerHandle ComboResetTimer;
	FTimerHandle MoveTimerStart;
	FTimerHandle CollisionDisableTimer;
	FTimerHandle TimerHandleArch;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//输入映射上下文
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)override;

	//获得攻击阶段
	int32 GetCurrentAttackStage() const { return CurrentAttackStage; }

	//输入映射菜单
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	//跳跃输入
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input1")
	TObjectPtr<UInputAction>JumpAction;

	//移动输入
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input1")
	TObjectPtr<UInputAction> MoveAction;

	//视角控制
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input1")
	TObjectPtr<UInputAction>LookAction;

	//奔跑
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input1")
	TObjectPtr<UInputAction> RunAction;

	//攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input1")
	TObjectPtr<UInputAction> AttackAction;

	//格挡
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input1")
	TObjectPtr<UInputAction> Block;

	//锁定
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Input1")
	TObjectPtr<UInputAction> LockOn;

	//踩刀
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Input1")
	TObjectPtr<UInputAction> Block_special;

	//突刺
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Input1")
	TObjectPtr<UInputAction> Block_Spike;

	//插值旋转速度
	UPROPERTY(BlueprintReadWrite, Category = "RotationSpeed")
	float RotationSpeed = 1.0f;

	//旋转角度
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float CurrentYaw;

	//状态
	UPROPERTY(BlueprintReadWrite, Category = "Character State")
	float State;
	
	//返回带方向速度
	UPROPERTY(BlueprintReadWrite, Category = "SpeedDirection")
	float SpeedDirection;

	//蒙太奇攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

	//蒙太奇格挡
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BlockMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage*Stop_BlockMontage;

	//蒙太奇弹反
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* HurtMontage;

	//蒙太奇被格挡R
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BlockedMontage;
	
	//蒙太奇被格挡L
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BlockedMontage_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BlockMontage1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BlockMontage2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage*BlockMontageStrong;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage*BlockMontageStrong2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage*BlockMontage_Special;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage*BlockMontageStill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ArchBlockMontage1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ArchBlockMontage2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Spike;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Spike_Block;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* BackwardsMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* Leap_ParryCombo1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	ASAICharacter* AIEnemy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	UMotionWarpingComponent* MotionWarpings;

	//玩家标签容器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer BlockTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector TargetLocation;
	
	UFUNCTION(BlueprintCallable, Category = "Combo")
	void OnAttack();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	bool NextAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void AttackMove();
	
	UFUNCTION(BlueprintCallable)
	void NotifyAI();

	UFUNCTION()
	void OnBlockedMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//开关敌人大碰撞体用以格挡预判
	UFUNCTION()
	void TurnCollisionNotify(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void StartBlockArch(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void StartParry_Player(FName NotifyName, const FBranchingPointNotifyPayload& Branch);
	
	UFUNCTION(BlueprintCallable, Category="Player")
	FVector GetHeadLocation() const;

	UFUNCTION(BlueprintCallable, Category="Player")
	FVector GetFootLocation() const;

	UFUNCTION()
	void OnLockOnTriggered(const FInputActionValue& Value);
	
	UFUNCTION()
	void Notify_PlayerPunch(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Notify_Playerthrust(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void AI_StillBlock(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void PlayerCombo(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void StartBlockArch_One(FName NotifyName, const FBranchingPointNotifyPayload& Branch);

	UFUNCTION()
	void Leap_ParryCombo(FName NotifyName, const FBranchingPointNotifyPayload& Branch);
	
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartRunning();

	void StopRunning();

	void EquipWeapon();

	void NextAttackTime();

	void ResetCombo();

	void Combo();

	void CanCombo();

	void StartBlock();

	void EndBlock();

	void PlayHurt();

	void DisableWeaponCollision();
	
	void AI_StopParry();

	void AI_StartParry();

	void OnBlocked();
	
	void OnBlocked_L();

	void ToggleLockOn();

	void PlayBlockAnimation();

	void PlayBlock_Special();

	void PlaySecondBlockArrowAnim();

	void PlaySpikeAnim();

	void BeginRetreat();

	void EndRetreat();
	
public:
	
	bool bIsBlock = false;

	float LastAttackTime = 0.0f; 
	const float AttackCooldown = 0.8f; 
};

