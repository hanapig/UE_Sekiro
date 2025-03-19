// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "SAnimInstance.h"
#include "MotionWarpingComponent.h"
#include "AI/NavigationSystemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


//初始化攻击
ASCharacter::ASCharacter():CurrentAttackStage(1)
{
	PrimaryActorTick.bCanEverTick = true;

	//蒙太奇动画名字
	NextName = "att1";
	BlockName = "block";
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = false; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 3000.0f, 0.0f); 

	//跳跃垂直加速度
	GetCharacterMovement()->JumpZVelocity = 700.f;
	//空中控制
	GetCharacterMovement()->AirControl = 0.35f;
	//行走速度
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	//最小模拟行走速度
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	//行走减速度
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true; 
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	UCapsuleComponent* MyCapsulete= GetCapsuleComponent();
	
	MotionWarpings = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASAICharacter::StaticClass(), FoundActors);
	
	if (FoundActors.Num() > 0)
	{
		AIEnemy = Cast<ASAICharacter>(FoundActors[0]);
	}

	UAnimInstance* EnemyAnimInstance = AIEnemy->GetMesh()->GetAnimInstance();
	
	//绑定蒙太奇通知
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::TurnCollisionNotify);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::Notify_PlayerPunch);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::AI_StillBlock);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::StartBlockArch);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::Notify_Playerthrust);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::StartBlockArch_One);
		
		
		EnemyAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::StartParry_Player);
		EnemyAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::PlayerCombo);
		EnemyAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::StartBlockArch_One);
		EnemyAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASCharacter::Leap_ParryCombo);
	}
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	EquipWeapon();

}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsLockOn && AIEnemy)
	{
		
		FRotator CurrentRotation = GetControlRotation();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),AIEnemy->GetActorLocation());
		
		float CameraRotationSpeed = 10.0f;
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation,TargetRotation,DeltaTime,CameraRotationSpeed);

		GetController()->SetControlRotation(NewRotation);
		
	}

	if (PlayerbIsMoving)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, PlayerTargetLocation, DeltaTime, PlayerInterpSpeed);
		
		SetActorLocation(NewLocation, true);
		
		if (FVector::Dist(NewLocation, PlayerTargetLocation) < 1.0f)
		{
			PlayerbIsMoving = false;
		}
	}
	USkeletalMeshComponent* AIEnemyMesh = AIEnemy->GetMesh();
	CapsuleLocation = AIEnemyMesh->GetSocketLocation(FName("Head"));

	if (bIsRetreating)
	{
	
		AddMovementInput(-GetActorForwardVector(), RetreatSpeed * DeltaTime);
	}
	
	
}


void ASCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		
		//跳跃
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//移动
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		
		//视角
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);

		//奔跑
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ASCharacter::StartRunning);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASCharacter::StopRunning);

		//攻击
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASCharacter::OnAttack);

		//格挡
		EnhancedInputComponent->BindAction(Block, ETriggerEvent::Triggered, this, &ASCharacter::StartBlock);
		EnhancedInputComponent->BindAction(Block, ETriggerEvent::Completed, this, &ASCharacter::EndBlock);

		//锁定
		EnhancedInputComponent->BindAction(LockOn, ETriggerEvent::Started, this, &ASCharacter::OnLockOnTriggered);

		//踩到
		EnhancedInputComponent->BindAction(Block_special, ETriggerEvent::Triggered, this, &ASCharacter::PlayBlock_Special);

		//突刺
		EnhancedInputComponent->BindAction(Block_Spike, ETriggerEvent::Triggered, this, &ASCharacter::PlaySpikeAnim);
	}

}


void ASCharacter::Move(const FInputActionValue& Value)
{
	float DeltaTimes = GetWorld()->GetDeltaSeconds();

	FVector2D MovementVector = Value.Get<FVector2D>();

	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	
	

	if (Controller != nullptr && (MovementVector.X != 0.0f || MovementVector.Y != 0.0f))
	{
		//速度和速度大小
		FVector CurrentVelocity = GetVelocity();
		float CurSpeed = CurrentVelocity.Size();
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("CurSpeed: %f"), CurSpeed));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("Velocity: %s"), *CurrentVelocity.ToString()));

		//控制器的初始朝向确定移动的前向和右向
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//根据输入向量计算目标方向
		FVector TargetDirection = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
		TargetDirection.Normalize();
		
		if (!TargetDirection.IsNearlyZero())
		{
			FRotator TargetRotation = TargetDirection.Rotation();
			float DeltaTime = GetWorld()->GetDeltaSeconds();
			FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
			SetActorRotation(NewRotation);
		}

		// 添加移动输入
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if (bIsRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = 1000.f;

			AnimInstance->State = 1.0f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.f;

			AnimInstance->State = 0.0f;
		}
		
		CurrentYaw = YawRotation.Yaw;

		//传递数据到动画实例
		if (AnimInstance)
		{
			AnimInstance->CurrentYaw = CurrentYaw;
		}

		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Current Yaw: %f"), GetActorRotation().Yaw));

		//计算速度方向
		if (CurSpeed > 300.0f)
		{
			FVector Acceleration = (CurrentVelocity - LastVelocity) / DeltaTimes;

			SpeedDirection= FVector::DotProduct(CurrentVelocity, Acceleration);

			if (AnimInstance)
			{
				AnimInstance->SpeedDirection = SpeedDirection;
			}
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("SpeedDirection: %f"), SpeedDirection));
		}

		LastVelocity = CurrentVelocity;

	}
}


void ASCharacter::Look(const FInputActionValue& Value)
{
	//鼠标方向
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//转向速度
	float YawScale = 0.25f;
	float PitchScale = 0.25f;

	//旋转角度
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	if (MyController)
	{
		APlayerCameraManager* CameraManager = MyController->PlayerCameraManager;
		if (CameraManager)
		{
			//设置视角的最大和最小俯仰角
			CameraManager->ViewPitchMin = -40.0f; 
			CameraManager->ViewPitchMax = 40.0f;  
		}
	}

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * YawScale);
		AddControllerPitchInput(LookAxisVector.Y * PitchScale);
	}
}

// 武器实例
void ASCharacter::EquipWeapon()
{
	if (WeaponClass)
	{
		
		FActorSpawnParameters SpawnParams;

		SpawnParams.Owner = this;

		SpawnParams.Instigator = GetInstigator();

		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParams);

		if (CurrentWeapon)
		{
			FName WeaponSocketName = "R_Weapon";  
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
			
		}
	}
	
}


void ASCharacter::OnAttack()
{
	
	
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (LastAttackTime == 0.0f) 
	{
		LastAttackTime = CurrentTime;
	}
	
	if (CurrentTime - LastAttackTime < AttackCooldown)
	{
		return; 
	}


	LastAttackTime = CurrentTime;
	
	if (bIsAttack && GetMesh() && GetMesh()->GetAnimInstance())
	{
		float MaxAttackRange = 200.0f;

		UE_LOG(LogTemp, Warning, TEXT("BlockTags: %s"), *BlockTags.ToString());

		// 确保 AIEnemy 指针有效后打印 AttackTags 中所有 tag
		if (AIEnemy)
		{
			UE_LOG(LogTemp, Warning, TEXT("AttackTags: %s"), *AIEnemy->AttackTags.ToString());
		}
		

		if (AIEnemy)
		{
			
			FVector DirectionToEnemy = CapsuleLocation - GetActorLocation();
			DirectionToEnemy.Normalize();  
			
			FVector WarpLocation = CapsuleLocation + DirectionToEnemy * 200.0f; 

			float DistanceToEnemy = FVector::Dist(GetActorLocation(), CapsuleLocation);
			
			if (DistanceToEnemy <= MaxAttackRange)
			{
				FMotionWarpingTarget NewWarpTarget;
				NewWarpTarget.Name = FName("att1");
				NewWarpTarget.Location = WarpLocation;
				MotionWarpings->AddOrUpdateWarpTarget(NewWarpTarget);
			}
		}
		
		
		if (NextName == "att1")
		{
			CurrentAttackStage = 1;
		}
		else if (NextName == "att2")
		{
			CurrentAttackStage = 2;
		}

		if (AIEnemy)
		{
			float DistanceToEnemy = FVector::Dist(GetActorLocation(), AIEnemy->GetActorLocation());
    
			if (DistanceToEnemy < 300.f &&
				!this->BlockTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Stiff"))) &&
				!this->BlockTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Hurt"))))
			{
				UE_LOG(LogTemp, Warning, TEXT("1"));
				UAnimInstance* EnemyAnimInstance = AIEnemy->GetMesh()->GetAnimInstance();
				EnemyAnimInstance->Montage_Play(AIEnemy->AI_Montage_Parry);
			}
		}

		
		
		PlayAnimMontage(AttackMontage, 1.0f, NextName);

		bIsAttack = false;
		
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &ASCharacter::NextAttackTime, 0.5f, false);

		bIsCombo = true;

		GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &ASCharacter::ResetCombo, 1.f, false);

		GetWorld()->GetTimerManager().SetTimer(ComboTimer, this, &ASCharacter::CanCombo, 0.7f, false);

		if (CurrentWeapon)
		{
			CurrentWeapon->StartBox();
		}

		// 关闭碰撞
		GetWorld()->GetTimerManager().SetTimer(CollisionDisableTimer, this, &ASCharacter::DisableWeaponCollision, 0.5f, false);

		//敌人检测攻击
		NotifyAI();
		
	}
}

//选择蒙太奇
bool ASCharacter::NextAttack()
{
	if (AttackMontage )  
	{
		switch (CurrentAttackStage)
		{
		case 1:
			NextName = "att2";
			return true;
		case 2:
			NextName = "att1";
			return false;
		default:
			NextName = "att1";
			return false;
		}
	}
	return false;

}


void ASCharacter::AttackMove()
{
	float SpeedMove = 400.0f;   
	float MoveTime = 0.08f;      
	float FirstTime = GetWorld()->GetTimeSeconds(); 

	
	FRotator ControlRotation = Controller ? Controller->GetControlRotation() : GetActorRotation();
	FVector MoveForward = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);  
	
	GetWorld()->GetTimerManager().SetTimer(MoveTimerStart, [this, MoveForward, SpeedMove, FirstTime, MoveTime]()
	{
		float CurTime = GetWorld()->GetTimeSeconds() - FirstTime;

		if (CurTime < MoveTime)
		{
			float DeltaTime = GetWorld()->GetDeltaSeconds();
			float Distance = SpeedMove * DeltaTime;

			FVector NewLocation = GetActorLocation() + (MoveForward * Distance);
			SetActorLocation(NewLocation);
			
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(MoveTimerStart);
		}
	}, 0.01f, true);  
}

void ASCharacter::NextAttackTime()
{
	bIsAttack = true;
	
}


void ASCharacter::Combo()
{
	if (bIsCombo )
	{
		
		if (NextAttack())
		{
			
			
			bIsCombo = true;
		}

		else
		{

			NextName = "att1";
			bIsCombo = true;
		}

	}
	else
	{
		
		NextName = "att1";
		bIsCombo = true;
	}
}


void ASCharacter::ResetCombo()
{
	
	NextName = "att1";
	bIsCombo = false;
}

void ASCharacter::CanCombo()
{
	if (bIsCombo)
	{
		if (NextAttack())
		{
			bIsCombo = true;

			
			GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
			
			GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &ASCharacter::ResetCombo, 1.0f, false);
		}
		else
		{
			ResetCombo();
		}
	}
}

void ASCharacter::StartBlock()
{

	if (bIsBlock)return;
	
	PlayAnimMontage(BlockMontage, 1.0f, BlockName);

	bIsBlock = true;

}

void ASCharacter::EndBlock()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance->Montage_IsPlaying(BlockMontage))
	{
		AnimInstance->Montage_Stop(0.2f, BlockMontage);
		PlayAnimMontage(Stop_BlockMontage, 1.0f);
	}

	bIsBlock = false;
}


void ASCharacter::PlayHurt()
{
	
	if( GetMesh() && GetMesh()->GetAnimInstance())
	{
		PlayAnimMontage(HurtMontage,1.0f);
	}
}


void ASCharacter::NotifyAI()
{
	//检测范围
	float Radius = 200.0f; 
	float Angle = 60.0f;   

	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();

	
}


void ASCharacter::StartRunning()
{
	bIsRunning = true;
	
}

void ASCharacter::StopRunning()
{
	bIsRunning = false;
	
}

void ASCharacter::DisableWeaponCollision()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopBox();
	}
}

void ASCharacter::TurnCollisionNotify(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{

		if(NotifyName==FName("AI_Block_Start"))
		{
			AI_StartParry();
		}
	
		if(NotifyName==FName("AI_Block_Stop"))
		{
			AI_StopParry();
		}
}

void ASCharacter::AI_StartParry()
{
	
	if (AIEnemy)
	{
		AIEnemy->StartParry();
	}
}

void ASCharacter::AI_StopParry()
{
	
	if (AIEnemy)
	{
		AIEnemy->StopParry();
	}

}


void ASCharacter::OnBlocked()
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{

		
		if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		}
		
		if (BlockedMontage)
		{
			PlayAnimMontage(BlockedMontage);
			
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->OnMontageEnded.RemoveDynamic(this, &ASCharacter::OnBlockedMontageEnded);
				
				AnimInstance->OnMontageEnded.AddDynamic(this, &ASCharacter::OnBlockedMontageEnded);
			}
		}
		
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->StopBox(); 
	}
	
	GetWorld()->GetTimerManager().ClearTimer(CollisionDisableTimer);
	
	bIsAttack = false;

	if (!GetWorld()->GetTimerManager().IsTimerActive(AttackTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &ASCharacter::NextAttackTime, 0.1f, false);
	}
}

void ASCharacter::OnBlocked_L()
{
	
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{

		if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		}
		
		if (BlockedMontage)
		{
			PlayAnimMontage(BlockedMontage_L);
			
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->OnMontageEnded.RemoveDynamic(this, &ASCharacter::OnBlockedMontageEnded);
				
				AnimInstance->OnMontageEnded.AddDynamic(this, &ASCharacter::OnBlockedMontageEnded);
			}
		}
		
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->StopBox(); 
	}
	
	GetWorld()->GetTimerManager().ClearTimer(CollisionDisableTimer);
	
	bIsAttack = false;
	
	if (!GetWorld()->GetTimerManager().IsTimerActive(AttackTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &ASCharacter::NextAttackTime, 0.1f, false);
	}
	
}


//2
void ASCharacter::OnBlockedMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == BlockedMontage|| Montage == BlockedMontage_L)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &ASCharacter::OnBlockedMontageEnded);
		}
		
		if (CurrentAttackStage == 1)
		{
			NextName = "att2";
		}
		else if (CurrentAttackStage == 2)
		{
			
			NextName = "att1";
		}
		else
		{
			NextName = "att1";
		}
		
		bIsAttack = false;
		
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
		
		if (!GetWorld()->GetTimerManager().IsTimerActive(AttackTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &ASCharacter::NextAttackTime, 0.1f, false);
		}

		GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &ASCharacter::ResetCombo, 0.8f, false);

		AIEnemy->StopParry();
	}
}

FVector ASCharacter::GetHeadLocation() const
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		return MeshComp->GetBoneLocation(FName("Head"));
	}

	return FVector::ZeroVector;
}

FVector ASCharacter::GetFootLocation() const
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		return MeshComp->GetBoneLocation(FName("L_Foot"));
	}

	return FVector::ZeroVector;
}

void ASCharacter::OnLockOnTriggered(const FInputActionValue& Value)
{
	
	ToggleLockOn();
}

void ASCharacter::ToggleLockOn()
{
	
	if (bIsLockOn)
	{
		bIsLockOn = false;
	}
	else
	{
		
		if (AIEnemy)
		{
			bIsLockOn = true;
		}
	}
}


void ASCharacter::PlayBlockAnimation()
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        
	
		if (AnimInstance->Montage_IsPlaying(BlockMontage1) ||
			AnimInstance->Montage_IsPlaying(BlockMontage2) ||
			AnimInstance->Montage_IsPlaying(BlockMontageStrong)||
			AnimInstance->Montage_IsPlaying(BlockMontageStrong2))
			
		{
			return; 
		}
        
		
		if (AIEnemy->AttackTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Rolling"))))
		{
			AIEnemy->StopParry();
			AIEnemy->StopAttack();
			PlayAnimMontage(BlockMontageStrong, 1.0f);
		}

		else if (AIEnemy->AttackTags.HasTag(FGameplayTag::RequestGameplayTag(FName("LeapAttack"))))
		{
			AIEnemy->StopParry();
			AIEnemy->StopAttack();
			PlayAnimMontage(BlockMontageStrong2, 1.0f);
			AIEnemy->AttackTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("LeapAttack")));
		}

		else if (AIEnemy->AttackTags.HasTag(FGameplayTag::RequestGameplayTag(FName("PlayerStill"))))
		{
			AIEnemy->StopParry();
			AIEnemy->StopAttack();
			PlayAnimMontage(BlockMontageStill, 1.0f);
			AIEnemy->AttackTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("PlayerStill")));
		}
		
		else
		{
			if (bIsBlock1Active)
			{

				PlayAnimMontage(BlockMontage1, 1.0f);
			}
			else
			{

				PlayAnimMontage(BlockMontage2, 1.0f);
			}
		}
        
		bIsBlock1Active = !bIsBlock1Active;
	}
}

void ASCharacter::Notify_PlayerPunch(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("PlayerPunch") || NotifyName == FName("PlayerPunchMedium") || NotifyName == FName("PlayerPunchStrong")||NotifyName== FName("PlayerPunchFollow"))
	{
		float ImpulseStrength = 50.0f;
        
		if (NotifyName == FName("PlayerPunch"))
		{
			ImpulseStrength = 60.0f;
		}
		else if (NotifyName == FName("PlayerPunchSmall"))
		{
			ImpulseStrength = 80.0f;
		}
		else if (NotifyName == FName("PlayerPunchStrong"))
		{
			ImpulseStrength = 300.0f;
		}

		else if (NotifyName == FName("PlayerPunchFollow"))
		{
			ImpulseStrength = 200.0f;
		}
		
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		
		FVector BackwardDirection = -GetActorForwardVector();
		BackwardDirection.Normalize();
		
		PlayerTargetLocation = GetActorLocation() + BackwardDirection * ImpulseStrength;
        
		PlayerbIsMoving = true;
	}
}


void ASCharacter::Notify_Playerthrust(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Playerthrust") || NotifyName == FName("PlayerthrustMedium") || NotifyName == FName("PlayerthrustStrong")||NotifyName== FName("PlayerthrustFollow"))
	{
		float ImpulseStrength = 0.0f;
        
		if (NotifyName == FName("Playerthrust"))
		{
			ImpulseStrength = 100.0f;
		}
		else if (NotifyName == FName("PlayerthrustSmall"))
		{
			ImpulseStrength = 50.0f;
		}
		else if (NotifyName == FName("PlayerthrustStrong"))
		{
			ImpulseStrength = 300.0f;
		}

		else if (NotifyName == FName("PlayerthrustFollow"))
		{
			ImpulseStrength = 200.0f;
		}
		
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		
		FVector BackwardDirection = GetActorForwardVector();
		BackwardDirection.Normalize();
		
		PlayerTargetLocation = GetActorLocation() + BackwardDirection * ImpulseStrength;
        
		PlayerbIsMoving = true;
	}
}



void ASCharacter::PlayBlock_Special()
{
	if (AIEnemy && AIEnemy->bCanwished && bCanPlayAnim)
	{
		if (GetMesh() && GetMesh()->GetAnimInstance())
		{
			PlayAnimMontage(BlockMontage_Special, 1.0f);
			
			bCanPlayAnim = false;
			
			AIEnemy->StartRecoilCooldown();
		}
	}
}


void ASCharacter::AI_StillBlock(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if(NotifyName==FName("AI_Still_Block"))
	{
		AIEnemy->PlayAnimMontage_StillCombo();
	}
}


void ASCharacter::StartBlockArch(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("StartBlock"))
	{
		bool bComboArch = AIEnemy->AttackTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Archery")));
		if (bComboArch && bIsBlock)
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{

				AnimInstance->StopAllMontages(0.1f);

				if (ArchBlockMontage2)
				{

					AnimInstance->Montage_Play(ArchBlockMontage2);
					
					GetWorld()->GetTimerManager().SetTimer(TimerHandleArch, this, &ASCharacter::PlaySecondBlockArrowAnim, 0.3, false);
				}
			}
		}
	}
}

void ASCharacter::PlaySecondBlockArrowAnim()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (ArchBlockMontage1)
		{
			AnimInstance->Montage_Play(ArchBlockMontage1);
			AIEnemy->AttackTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Archery")));
		}
	}
}

void ASCharacter::PlaySpikeAnim()
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
	
		PlayAnimMontage(Spike, 1.0f);
	
		FTimerHandle TimerHandles;
		GetWorldTimerManager().SetTimer(TimerHandles, FTimerDelegate::CreateLambda([this]()
		{
			if (AIEnemy)
			{
				AIEnemy->OnParried_Spike();
			}
		}), 0.05f, false);
	}
}


void ASCharacter::StartParry_Player(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Parry_Spike"))
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		PlayAnimMontage(Spike_Block, 1.0f);
	}
}


void ASCharacter::PlayerCombo(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("PlayerCombo"))
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		PlayAnimMontage(BlockMontage1, 1.0f);
	}
}


void ASCharacter::StartBlockArch_One(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("StartBlock_One"))
	{
		
		if (bIsBlock)
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{

				AnimInstance->StopAllMontages(0.1f);

				if (ArchBlockMontage2)
				{
					AnimInstance->Montage_Play(ArchBlockMontage2);
				}
				
			}
		}
	}
}



void ASCharacter::BeginRetreat()
{
	
	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->Montage_Play(BackwardsMontage);
	bIsRetreating = true;

	
}

void ASCharacter::EndRetreat()
{
	
	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->Montage_Stop(0.1f, BackwardsMontage);
	bIsRetreating = false;

	
}



void ASCharacter::Leap_ParryCombo(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Leap_ParryCombo"))
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		PlayAnimMontage(Leap_ParryCombo1, 1.0f);
	}
}

