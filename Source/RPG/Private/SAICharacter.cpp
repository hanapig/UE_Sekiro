// Fill out your copyright notice in the Description page of Project Settings.

#include "SAICharacter.h"

#include <RootMotionModifier.h>

#include "SAIAnimInstance.h"
#include "SCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BTTask_Circle.h"
#include "AI/BTTask_Stop.h"
#include "Kismet/GameplayStatics.h"

ASAICharacter::ASAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	
}


void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AIMesh = FindComponentByClass<USkeletalMeshComponent>();
	
	UBoxComponent* AIBox = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	UBoxComponent* PBox = Cast<UBoxComponent>(FindComponentByTag(UBoxComponent::StaticClass(), FName("PBox")));
	
	UCapsuleComponent* CollisionCylinder = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));

	if (!Player)
	{
		ACharacter* TempPlayer = UGameplayStatics::GetPlayerCharacter(this, 0);
		Player = Cast<ASCharacter>(TempPlayer);
	}

	if (!Player)
	{
		return;
	}

	if (!Player->GetMesh())
	{
		return;
	}

	UAnimInstance* PlayerAnimInstance = Player->GetMesh()->GetAnimInstance();
	if (!PlayerAnimInstance)
	{
		return;
	}
	

	if (PBox)
	{
		
		PBox->SetCollisionObjectType(ECC_Pawn);
		PBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		PBox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		PBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
		PBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PBox->SetGenerateOverlapEvents(true);
		PBox->SetVisibility(false);
		PBox->ComponentTags.Add(FName("PBox"));
	}
	
	if (AIBox && AIMesh)
	{
		AIBox->AttachToComponent(AIMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("R_Weap"));
		AIBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	}
	
	if (AIBox)
	{
		// 设置碰撞属性
		AIBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		AIBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AIBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
		AIBox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AIBox->OnComponentBeginOverlap.AddDynamic(this, &ASAICharacter::OnAI_AttackHit);
		
	}

	UAnimInstance* AnimInstance =GetMesh()->GetAnimInstance();

	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::AttackMontageNotify);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_NotifySuccess);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_Punch);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_UP);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::SetCanwishedStart);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::SetCanwishedStop);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::StopCollision);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Startretreating);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Stopretreating);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::StartParry_Animontion);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::StartCombo);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::BCanCombo);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_Left);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_Right);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::ComboStill);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::ComboSArch);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Spike_Block_Combo);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Combo3);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_Right_Run);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_ForWard_Run);
		
		
		PlayerAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_Blocked_l);
		PlayerAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_Blocked_l2);
		PlayerAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Notify_Blocked_Special);
		PlayerAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Combo4);
		PlayerAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAICharacter::Combo5);
		
	}

	
}

void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Player)
	{
		PlayerHeadLocation = Player->GetHeadLocation();
	}
	
	if (Player)
	{
		PlayerFootLocation = Player->GetFootLocation();
	}

	if (bIsArcMoving && Player)
	{
		MoveElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(MoveElapsedTime / MoveDuration, 0.0f, 1.0f);

		float TargetRadius = FMath::Max((GetActorLocation() - ArcCenter).Size(), MinimumRadius);
		Radius = FMath::Lerp(Radius, TargetRadius, DeltaTime / SmoothRadiusTime);
		
	
		float CurrentAngleDegrees ;
		
		if (bIsRightArc)
		{
			CurrentAngleDegrees = FMath::Lerp(0.0f, 90.0f, Alpha); 
		}
		else
		{
			CurrentAngleDegrees = FMath::Lerp(0.0f, -90.0f, Alpha); 
		}
        
	
		FVector PlayerForward = Player->GetActorForwardVector();
		FVector CurrentOffset = PlayerForward.RotateAngleAxis(CurrentAngleDegrees, FVector::UpVector);
        
	
		FVector NewPosition = ArcCenter + CurrentOffset * Radius;
		SetActorLocation(NewPosition, true);
        
		FVector DirectionToPlayer = ArcCenter - NewPosition;
		FRotator NewRotation = DirectionToPlayer.Rotation();
		SetActorRotation(NewRotation);
        
		if (Alpha >= 1.0f)
		{
			bIsArcMoving = false;

			ACharacter* EnemyCharacter = this;
			EnemyCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		}
	}

	else if (bIsMoving)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
		SetActorLocation(NewLocation, true);
        
		if (FVector::Dist(NewLocation, TargetLocation) < 1.0f)
		{
			bIsMoving = false;
		}
	}

	if (bIsMovingRight)
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime <= MoveTime)
		{
			FVector NewLocation = StartLocation + FVector(0.0f, -Speed * ElapsedTime, 0.0f);
			SetActorLocation(NewLocation);
		}
		else
		{
			// 右跑结束后更新起始位置
			StartLocation = GetActorLocation();
			bIsMovingRight = false;
		}
	}


	if (bIsMovingForward)
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime <= MoveTime)
		{
			float Alpha = ElapsedTime / MoveTime;
			// 从起始位置到目标位置的直线插值
			FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
			SetActorLocation(NewLocation);
            
			// 可选：更新角色朝向，使其面向运动方向
			FVector MoveDirection = (TargetLocation - StartLocation).GetSafeNormal();
			SetActorRotation(MoveDirection.Rotation());
		}
		else
		{
			// 运动结束后确保精确到达目标位置
			SetActorLocation(TargetLocation);
			bIsMovingForward = false;
		}
	}

	
}


void ASAICharacter::AttackMontageNotify(FName NotifyName,const FBranchingPointNotifyPayload& Branch)
{
	
	if(NotifyName==FName("AI_Attack_Start"))
	{
		StartAttack();
	}

	if(NotifyName==FName("AI_Attack_End"))
	{
		StopAttack();
	}
}

void ASAICharacter::OnParried()
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		 if (AI_Montage_Parry)
		{
			
			PlayAnimMontage(AI_Montage_Parry);
		}
		
	}
}


void ASAICharacter::OnParried_Spike()
{

	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		if (Blocked_Combo2)
		{
			PlayAnimMontage(Blocked_Combo2);
		}
		
	}
}

void ASAICharacter::StartParry_Animontion(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	
	AAIController* AIController = Cast<AAIController>(GetController());
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();

	bool bSpike = BlackboardComp->GetValueAsBool("Spike");

	bool bHasHurtTag = Player->BlockTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Hurt")));

	if (!bSpike&&StaggerHitCount ==2)
	{
		if (NotifyName == FName("StartParry"))
		{
			
			PlayAnimMontage(Blocked_l3);
			BlackboardComp->SetValueAsBool("Warring", true);
			StaggerHitCount=0;
		}
	}
}


void ASAICharacter::StartCombo(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("StartCombo"))
	{
		FName FunctionName("StartCombo_Spike");
		if (UFunction* Func = this->FindFunction(FunctionName))
		{
			this->ProcessEvent(Func, nullptr);
		}
	}
}


void ASAICharacter::BCanCombo(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{

	if (NotifyName == FName("bCanCombo"))
	{

		AAIController* AIController = Cast<AAIController>(GetController());
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		BlackboardComp->SetValueAsBool("bCanCombo", true);
		
	}
}


void ASAICharacter::OnInjured()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	
	this->AttackTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Combo3")));
	PlayAnimMontage(InjuredMontage);
	
	if (bIsInStaggerState)
	{
		
		StaggerHitCount++;
		if (StaggerHitCount ==2)
		{
			UGameplayStatics::PlaySoundAtLocation(this, InjurySound, GetActorLocation());
			Player->BlockTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Hurt")));
			bIsInStaggerState = false;
	
		}
	}
	
	else
	{
		Player->BlockTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Hurt")));
	}
}


void ASAICharacter::OnParried_L()
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AI_Montage_Parry_L)
		{
			UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
            
			PlayAnimMontage(AI_Montage_Parry_L);
			
			ParryCount++;
			
			if(ParryCount == 2)
			{
				BlackboardComp->SetValueAsBool("bCan_LeapAttack", true);
			}
		}
	}
}


void ASAICharacter::StartAttack()
{
	AIMesh = FindComponentByClass<USkeletalMeshComponent>();
	UBoxComponent* AIBox = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	
	if (AIBox)
	{
		AIBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AIBox->SetVisibility(true);
		
	}

}

void ASAICharacter::StopAttack()
{
	AIMesh = FindComponentByClass<USkeletalMeshComponent>();
	UBoxComponent* AIBox = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	
	if (AIBox)
	{
		AIBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AIBox->SetVisibility(false);
		
	}
}


void ASAICharacter::StartParry()
{
	UBoxComponent* PBox = Cast<UBoxComponent>(FindComponentByTag(UBoxComponent::StaticClass(), FName("PBox")));
	if (PBox)
	{
		PBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PBox->SetVisibility(true);
	}

}


void ASAICharacter::StopParry()
{
	
	UBoxComponent* PBox = Cast<UBoxComponent>(FindComponentByTag(UBoxComponent::StaticClass(), FName("PBox")));
	if (PBox)
	{
		PBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PBox->SetVisibility(false);
	}
}


void ASAICharacter::OnAI_AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if(OtherActor && OtherActor->IsA<ASCharacter>())
	{
		
		ASCharacter* Enemy = Cast<ASCharacter>(OtherActor);

		if (Player->bIsBlock)
		{
			Player->PlayBlockAnimation();
		}
		
		else 
		{
			
			FVector EnemyLocation = Enemy->GetActorLocation();
			
			AI_Attack(Enemy, EnemyLocation);
		}
	}
}

void ASAICharacter::AI_Attack(AActor*PlayerActor,const FVector& PlayerLocation)
{
	
		Player->PlayHurt();
		bBlockedTriggered = false;
}


void ASAICharacter::Notify_NotifySuccess(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Notify_Success"))
	{
		
		UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(GetController()->GetComponentByClass(UBehaviorTreeComponent::StaticClass()));
		
		if (BehaviorComp)
		{
			
			const UBTNode* ActiveNode = BehaviorComp->GetActiveNode();
            
			if (ActiveNode)
			{
				
				const UBTTaskNode* TaskNode = Cast<UBTTaskNode>(ActiveNode);
                
				if (TaskNode)
				{
					TaskNode->FinishLatentTask(*BehaviorComp, EBTNodeResult::Succeeded);

					const float DelayTime = 0.3f; 
					GetWorld()->GetTimerManager().SetTimer(SetStop, this, &ASAICharacter::StopAttack, DelayTime, false);
				}
			}
		}
	}
}

void ASAICharacter::Notify_Punch(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	
	if (NotifyName == FName("Punch") || NotifyName == FName("PunchMedium") || NotifyName == FName("PunchStrong")||NotifyName == FName("PunchAISmall"))
	{
		float ImpulseStrength = 0.0f;
        
		if (NotifyName == FName("Punch"))
		{
			ImpulseStrength = 100.0f;
		}
		else if (NotifyName == FName("PunchMedium"))
		{
			ImpulseStrength = 130.0f;
		}
		else if (NotifyName == FName("PunchStrong"))
		{
			ImpulseStrength = 300.0f;
		}
		
		else if (NotifyName == FName("PunchAISmall"))
		{
			ImpulseStrength = 50.0f;
		}
        

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        

		FVector BackwardDirection = -GetActorForwardVector();
		BackwardDirection.Normalize();
        

		TargetLocation = GetActorLocation() + BackwardDirection * ImpulseStrength;
        
		bIsMoving = true;
	}
}

void ASAICharacter::Notify_Left(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Left"))
	{
		if (Player)
		{
			ArcCenter = Player->GetActorLocation();
			Radius = (GetActorLocation() - ArcCenter).Size();

			bIsMoving = false;
			bIsArcMoving = true;

		
			bIsRightArc = false; 
		}
	}
}



void ASAICharacter::Notify_Right(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Right"))
	{
		if (Player)
		{
			ArcCenter =GetActorLocation();
			Radius = (GetActorLocation() - ArcCenter).Size();

			bIsMoving = false;
			bIsArcMoving = true;


			bIsRightArc = true;   
		}
	}
}




void ASAICharacter::Notify_Blocked_l(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	
	if (NotifyName == FName("AI_Blocked")&&!AttackTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Combo"))))
	{
		if (GetMesh() && GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			
			AnimInstance->Montage_Stop(0.2f);
			
			PlayAnimMontage(Blocked_l, 1.0f);
		}
	}
}

void ASAICharacter::Notify_Blocked_l2(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("AI_Blocked1")&&!AttackTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Combo"))))
	{
		if (GetMesh() && GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			
			AnimInstance->Montage_Stop(0.2f);
			
			PlayAnimMontage(Blocked_l2, 1.0f);
		}
	}
}


void ASAICharacter::Notify_UP(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("UP"))
	{
		if (!GetCharacterMovement()->IsFalling()) 
		{
			
			GetCharacterMovement()->Launch(FVector(0, 0, 1) * 500.f); 
			
		}
	}
}


void ASAICharacter::Notify_Right_Run(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Right_Run"))
	{
		Speed = 700.0f;    
		MoveTime = 0.6f;   
		ElapsedTime = 0.0f;
		StartLocation = GetActorLocation();
		bIsMovingRight = true;
	}
}

void ASAICharacter::Notify_ForWard_Run(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Forward_Run"))
	{
		Speed = 400.0f;    
		MoveTime = 0.8f;   
		ElapsedTime = 0.0f;
		// 使用当前的位置作为前跑起点（右跑完成后的位置）
		StartLocation = GetActorLocation();

		// 纯前跑方向（假设为全局 X 轴正方向）
		FVector PureForward = FVector(1, 0.4, 0);

		// 计算从起点到玩家的方向（忽略 Z 轴）
		FVector ToPlayer = Player->GetActorLocation() - StartLocation;
		ToPlayer.Z = 0.0f;
		ToPlayer.Normalize();

		// 设定倾斜因子，控制倾斜程度（0 表示不倾斜，1 表示完全朝向玩家）
		float TiltFactor = 0.7f; // 可根据需要调整

		// 混合方向
		FVector BlendedDir = (PureForward + TiltFactor * ToPlayer).GetSafeNormal();

		// 根据混合方向和预期跑动距离计算目标位置
		TargetLocation = StartLocation + BlendedDir * (Speed * MoveTime);
        
		bIsMovingForward = true;
	}
}



void ASAICharacter::SetCanwishedStart(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Start"))
	{
		if (!bCanwished) 
		{
			bCanwished = true;
			
			StartRecoilCooldown();

			if (GEngine)
			{
				FString BoolAsString = bCanwished ? TEXT("True") : TEXT("False");
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("bCanwished is: %s"), *BoolAsString));
			}
		}
	}
}

void ASAICharacter::SetCanwishedStop(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Stop"))
	{
		bCanwished= false;

		StartRecoilCooldown();
		
		if (GEngine)
		{
			FString BoolAsString = bCanwished ? TEXT("True") : TEXT("False");
			
		}
	}
}

void ASAICharacter::StartRecoilCooldown()
{

	GetWorld()->GetTimerManager().SetTimer(ReSpecialTime, [this]() {
		bCanwished = false;
	}, ReSpecial, false);
}


void ASAICharacter::Notify_Blocked_Special(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Blocked_Special"))
	{
		Player->BlockTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Hurt")));
		
		if (GetMesh() && GetMesh()->GetAnimInstance())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (UMotionWarpingComponent* MotionWarpings = FindComponentByClass<UMotionWarpingComponent>())
			{
				WarpLocation=Player->GetActorLocation();
				FMotionWarpingTarget NewWarpTarget;
				NewWarpTarget.Name = FName("point5");  
				NewWarpTarget.Location =WarpLocation;  
				MotionWarpings->AddOrUpdateWarpTarget(NewWarpTarget);
			}
			
			AnimInstance->Montage_Stop(0.2f);
			
			PlayAnimMontage(Blocked_Special, 1.0f);
			

			bIsInStaggerState = true;
			StaggerHitCount = 0;
			
		}
	}
}


void ASAICharacter::StopCollision(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("StopCollision"))
	{
		StopAttack();
		StopParry();
	}
}


void ASAICharacter::Startretreating(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Startretreating"))
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	
        BlackboardComp->SetValueAsBool("retreating", true);
		
	}
}


void ASAICharacter::Stopretreating(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Stopretreating"))
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	
		BlackboardComp->SetValueAsBool("retreating", false);
		
	}
}


void ASAICharacter::ComboStill(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("PlayerStill"))
	{
		AttackTags.AddTag(FGameplayTag::RequestGameplayTag(FName("PlayerStill")));
	}
}

void ASAICharacter::PlayAnimMontage_StillCombo()
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			
		AnimInstance->Montage_Stop(0.2f);
			
		PlayAnimMontage(Blocked_Combo, 1.0f);
	}
}


void ASAICharacter::ComboSArch(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("ComboArch"))
	{
		AttackTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Archery")));
	}
}


void ASAICharacter::Spike_Block_Combo(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Parry_Spike"))
	{
	
		PlayAnimMontage(Blocked_Combo3, 1.0f);
		
	}
}



void ASAICharacter::Combo3(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	if (NotifyName == FName("Combo3"))
	{

		PlayAnimMontage(Blocked_l2, 1.0f);
		
	}
}


void ASAICharacter::Combo4(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	bool Combo4=AttackTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Combo3")));
	
	if (NotifyName == FName("Combo4_Parry")&&Combo4)
	{

		PlayAnimMontage(Blocked_Combo4, 1.0f);
		
	}
}



void ASAICharacter::Combo5(FName NotifyName, const FBranchingPointNotifyPayload& Branch)
{
	bool Combo4=AttackTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Combo3")));
	
	if(Combo4)
	{
		if (NotifyName == FName("PlayerCombo4"))
		{
			
			UE_LOG(LogTemp, Display, TEXT("PlayerCombo4"));
			
			Player->BlockTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Stiff")));
			Player->BlockTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Hurt")));
			
		}
		
	}
	
}


