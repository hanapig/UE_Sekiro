#include "CommonTools.h"
#include "NiagaraComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

UCommonTools* UCommonTools::GetCommonToolsSubsystem(UObject* WorldContextObject)
{
    if (!WorldContextObject) return nullptr;
    UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
    if (!World) return nullptr;
    return World->GetSubsystem<UCommonTools>();
}

// 根据标签获取对应的角色数组
const TArray<AActor*>& UCommonTools::GetCharactersByTag(const FName& Tag)
{
    static TArray<AActor*> Empty;
    if (TArray<AActor*>* Found = CharacterMap.Find(Tag))
        return *Found;
    return Empty;
}

// 添加角色到指定标签的映射中
void UCommonTools::AddCharacterToMap(AActor* Character, const FName& Tag)
{
    CharacterMap.FindOrAdd(Tag).Add(Character);
}



FString UCommonTools::GetDirection(float Yaw, const TArray<EImpactDirection>& IgnoreDirections)
{
    struct FDirEntry
    {
        EImpactDirection Dir;
        float CenterDeg;
        const TCHAR* Name;
    };

    static const FDirEntry Entries[] =
    {
        { EImpactDirection::Front,  0.f,   TEXT("Front") },
        { EImpactDirection::Right,  90.f,  TEXT("Right") },
        { EImpactDirection::Back,   180.f, TEXT("Back")  },
        { EImpactDirection::Left,  -90.f,  TEXT("Left")  },
    };

    TSet<EImpactDirection> Ignored;
    Ignored.Reserve(IgnoreDirections.Num());
    for (EImpactDirection D : IgnoreDirections)
    {
        if (D != EImpactDirection::None)
        {
            Ignored.Add(D);
        }
    }

    float BestDelta = TNumericLimits<float>::Max();
    const TCHAR* BestName = TEXT("None");
    bool bFound = false;

    for (const FDirEntry& E : Entries)
    {
        if (Ignored.Contains(E.Dir))
            continue;

        float Delta = FMath::Abs(FMath::FindDeltaAngleDegrees(Yaw, E.CenterDeg));
        if (E.Dir == EImpactDirection::Back)
        {
            float Alt = FMath::Abs(FMath::FindDeltaAngleDegrees(Yaw, -180.f));
            Delta = FMath::Min(Delta, Alt);
        }

        if (Delta < BestDelta)
        {
            BestDelta = Delta;
            BestName = E.Name;
            bFound = true;
        }
    }

    return FString(bFound ? BestName : TEXT("None"));
}

EImpactDirection UCommonTools::GetDirectionByNearly(float Yaw, const TArray<EImpactDirection>& IgnoreDirections)
{
    struct FDirEntry
    {
        EImpactDirection Dir;
        float CenterDeg;
    };

    static const FDirEntry Entries[] =
    {
        { EImpactDirection::Front,  0.f   },
        { EImpactDirection::Right,  90.f  },
        { EImpactDirection::Back,   180.f },
        { EImpactDirection::Left,  -90.f  },
    };

    TSet<EImpactDirection> Ignored;
    Ignored.Reserve(IgnoreDirections.Num());
    for (EImpactDirection D : IgnoreDirections)
    {
        if (D != EImpactDirection::None)
        {
            Ignored.Add(D);
        }
    }

    float BestDelta = TNumericLimits<float>::Max();
    EImpactDirection BestDir = EImpactDirection::None;
    bool bFound = false;

    for (const FDirEntry& E : Entries)
    {
        if (Ignored.Contains(E.Dir))
            continue;

        float Delta = FMath::Abs(FMath::FindDeltaAngleDegrees(Yaw, E.CenterDeg));
        if (E.Dir == EImpactDirection::Back)
        {
            float Alt = FMath::Abs(FMath::FindDeltaAngleDegrees(Yaw, -180.f));
            Delta = FMath::Min(Delta, Alt);
        }

        if (Delta < BestDelta)
        {
            BestDelta = Delta;
            BestDir = E.Dir;
            bFound = true;
        }
    }

    return bFound ? BestDir : EImpactDirection::None;
}


// 检查Actor是否包含指定的Socket
bool UCommonTools::HasSocket(AActor* Actor, FName SocketName)
{
    if (!Actor || SocketName.IsNone()) return false;
    
    USkeletalMeshComponent* MeshComp = Actor->FindComponentByClass<USkeletalMeshComponent>();
    if (!MeshComp || !MeshComp->GetSkeletalMeshAsset()) return false;
    
    return MeshComp->GetSkeletalMeshAsset()->GetRefSkeleton().FindBoneIndex(SocketName) != INDEX_NONE ||
           MeshComp->DoesSocketExist(SocketName);
}

// 根据枚举类型获取对应的Socket名称
FName UCommonTools::GetSocketNameFromType(ESocketName SocketType)
{
    switch (SocketType)
    {
    case ESocketName::Root:      return TEXT("Root");
    case ESocketName::Spine:     return TEXT("Spine");
    case ESocketName::Head:      return TEXT("Head");
    case ESocketName::HandLeft:  return TEXT("Hand_l");
    case ESocketName::HandRight: return TEXT("Hand_r");
    case ESocketName::FootLeft:  return TEXT("Foot_l");
    case ESocketName::FootRight: return TEXT("Foot_r");
    case ESocketName::Custom:    return NAME_None;
    default:                     return TEXT("Root");
    }
}

EImpactDirection UCommonTools::ImpactDirection(FVector DamageDealerVector,
                                               FVector DamageReceiverVector,
                                               float FrontHalfAngleDeg,
                                               float RightHalfAngleDeg,
                                               float BackHalfAngleDeg,
                                               FVector ReceiverForward)
{
    //受击向量标准化
    FVector ToDealer = DamageDealerVector - DamageReceiverVector;
    ToDealer.Z = 0.f;
    if (ToDealer.IsNearlyZero())
        return EImpactDirection::Front;
    ToDealer.Normalize();

    //受击者正前方向量标准化
    FVector Forward = ReceiverForward;
    Forward.Z = 0.f;
    if (Forward.IsNearlyZero())
        Forward = FVector(1.f, 0.f, 0.f);
    Forward.Normalize();

    //点积叉积判断前后左右，转化为角度
    float Dot = FVector::DotProduct(Forward, ToDealer);
    Dot = FMath::Clamp(Dot, -1.f, 1.f);
    float CrossZ = FVector::CrossProduct(Forward, ToDealer).Z;
    float SignedAngleRad = FMath::Atan2(CrossZ, Dot);
    float AngleDeg = FMath::RadiansToDegrees(SignedAngleRad); 
    // 按阈值判断方向（可配置）
    float frontMin = -FrontHalfAngleDeg;          
    float frontMax = FrontHalfAngleDeg;           
    float rightMin = frontMax;                    
    float rightMax = frontMax + RightHalfAngleDeg;
    float backPosMin = 180.f - BackHalfAngleDeg;  
    float backNegMax = -(180.f - BackHalfAngleDeg);

    if (AngleDeg > frontMin && AngleDeg <= frontMax)
        return EImpactDirection::Front;

    if (AngleDeg > rightMin && AngleDeg <= rightMax)
        return EImpactDirection::Right;

    if (AngleDeg > backPosMin && AngleDeg <= 180.f)
        return EImpactDirection::Back;

    if (AngleDeg > -180.f && AngleDeg <= backNegMax)
        return EImpactDirection::Back;

    return EImpactDirection::Left;
}



bool UCommonTools::ForceFaceTarget(AActor* Owner, AActor* Attacker, bool bInstant, float InterpSpeed, bool bSyncController, float TempRotateDuration)
{
    if (!Owner || !Attacker || Owner == Attacker) return false;

    FVector Dir = Attacker->GetActorLocation() - Owner->GetActorLocation();
    Dir.Z = 0.f;
    if (Dir.IsNearlyZero()) return false;

    // 目标朝向 (仅 Yaw)
    FRotator TargetRot = Dir.Rotation();
    TargetRot.Pitch = 0.f;
    TargetRot.Roll  = 0.f;

    // 计算新朝向
    FRotator NewRot;
    if (bInstant || InterpSpeed <= 0.f)
    {
        NewRot = TargetRot;
    }
    else
    {
        const float Delta = Owner->GetWorld() ? Owner->GetWorld()->GetDeltaSeconds() : (1.f / 60.f);
        NewRot = FMath::RInterpTo(Owner->GetActorRotation(), TargetRot, Delta, InterpSpeed);
        NewRot.Pitch = 0.f;
        NewRot.Roll  = 0.f;
    }

    // 判断是否需要旋转 
    const bool bWillRotate = !Owner->GetActorRotation().Equals(NewRot, 0.01f);

    // 处理角色临时关闭自动朝向
    if (ACharacter* Char = Cast<ACharacter>(Owner))
    {
        bool bOldUseControllerYaw = Char->bUseControllerRotationYaw;
        bool bOldOrientToMovement = false;

        if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
        {
            bOldOrientToMovement = Move->bOrientRotationToMovement;
            Move->bOrientRotationToMovement = false;
        }

        if (bSyncController)
            Char->bUseControllerRotationYaw = true;

        // 定时恢复
        if (TempRotateDuration > 0.f && Owner->GetWorld())
        {
            TWeakObjectPtr<ACharacter> WeakChar = Char;
            FTimerDelegate RestoreDelegate = FTimerDelegate::CreateLambda(
                [WeakChar, bOldUseControllerYaw, bOldOrientToMovement]()
                {
                    if (ACharacter* C = WeakChar.Get())
                    {
                        if (UCharacterMovementComponent* M = C->GetCharacterMovement())
                            M->bOrientRotationToMovement = bOldOrientToMovement;
                        C->bUseControllerRotationYaw = bOldUseControllerYaw;
                    }
                });
            FTimerHandle Handle;
            Owner->GetWorld()->GetTimerManager().SetTimer(Handle, RestoreDelegate, TempRotateDuration, false);
        }
    }

    // 应用旋转
    if (bWillRotate)
    {
        Owner->SetActorRotation(NewRot, ETeleportType::TeleportPhysics);
    }

    return bWillRotate;
}


bool UCommonTools::ForceFaceTargetByDirection(AActor* Owner,
                                   AActor* Attacker,
                                   const FString& Direction,
                                   bool bInstant,
                                   float InterpSpeed,
                                   bool bSyncController,
                                   float TempRotateDuration)
{
    if (!Owner || !Attacker || Owner == Attacker) return false;

    //攻击者位置与受击者位置的方向向量
    FVector Dir = Attacker->GetActorLocation() - Owner->GetActorLocation();
    Dir.Z = 0.f;
    if (Dir.IsNearlyZero()) return false;
    
    FRotator TargetRot = Dir.Rotation();
    TargetRot.Pitch = 0.f;
    TargetRot.Roll  = 0.f;

    // 根据方向字符串调整目标朝向
    int32 OffsetYaw = 0;
    //不区分大小写
    FString Lower = Direction.ToLower();
    if (Lower == TEXT("front"))
        OffsetYaw = 0;
    else if (Lower == TEXT("back"))
        OffsetYaw = 180;
    else if (Lower == TEXT("left"))
        OffsetYaw = 90;
    else if (Lower == TEXT("right"))
        OffsetYaw = -90;
    TargetRot.Yaw = FRotator::NormalizeAxis(TargetRot.Yaw + OffsetYaw);

    // 计算新朝向
    FRotator NewRot;
    if (bInstant || InterpSpeed <= 0.f)
    {
        NewRot = TargetRot;
    }
    else
    {
        const float Delta = Owner->GetWorld() ? Owner->GetWorld()->GetDeltaSeconds() : (1.f / 60.f);
        NewRot = FMath::RInterpTo(Owner->GetActorRotation(), TargetRot, Delta, InterpSpeed);
        NewRot.Pitch = 0.f;
        NewRot.Roll  = 0.f;
    }

    const bool bWillRotate = !Owner->GetActorRotation().Equals(NewRot, 0.01f);

    if (ACharacter* Char = Cast<ACharacter>(Owner))
    {
        bool bOldUseControllerYaw = Char->bUseControllerRotationYaw;
        bool bOldOrientToMovement = false;

        if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
        {
            bOldOrientToMovement = Move->bOrientRotationToMovement;
            Move->bOrientRotationToMovement = false;
        }

        if (bSyncController)
            Char->bUseControllerRotationYaw = true;

        if (TempRotateDuration > 0.f && Owner->GetWorld())
        {
            TWeakObjectPtr<ACharacter> WeakChar = Char;
            FTimerDelegate RestoreDelegate = FTimerDelegate::CreateLambda(
                [WeakChar, bOldUseControllerYaw, bOldOrientToMovement]()
                {
                    if (ACharacter* C = WeakChar.Get())
                    {
                        if (UCharacterMovementComponent* M = C->GetCharacterMovement())
                            M->bOrientRotationToMovement = bOldOrientToMovement;
                        C->bUseControllerRotationYaw = bOldUseControllerYaw;
                    }
                });
            FTimerHandle Handle;
            Owner->GetWorld()->GetTimerManager().SetTimer(Handle, RestoreDelegate, TempRotateDuration, false);
        }
    }

    if (bWillRotate)
    {
        Owner->SetActorRotation(NewRot, ETeleportType::TeleportPhysics);
    }

    return bWillRotate;
}

void UCommonTools::DestroyComponentManual(UObject* Component)
{
    if (!Component) return;
    
    if (UNiagaraSystem* NiagaraSys = Cast<UNiagaraSystem>(Component))
    {
        for (TObjectIterator<UNiagaraComponent> It; It; ++It)
        {
            UNiagaraComponent* Comp = *It;
            if (!Comp) continue;
            if (Comp->GetAsset() == NiagaraSys)
            {
                Comp->DeactivateImmediate();
                Comp->DestroyComponent();
            }
        }
        return;
    }
    
    if (AActor* Actor = Cast<AActor>(Component))
    {
        Actor->Destroy();
        return;
    }
    
    if (UActorComponent* ActorComp = Cast<UActorComponent>(Component))
    {
        if (UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(ActorComp))
        {
            NiagaraComp->DeactivateImmediate();
            NiagaraComp->DestroyComponent();
        }
        else
        {
            ActorComp->Deactivate();
            ActorComp->DestroyComponent();
        }
    }
}

bool UCommonTools::UpdatePressCondition(UInputAction* Action, float ThresholdSeconds, bool bIsPressed)
{
    // 无效输入动作或阈值直接返回
    FHoldPressState& State = HoldStates.FindOrAdd(TWeakObjectPtr<UInputAction>(Action));
    UWorld* World = GetWorld();
    if (!World) return true;

    // 按下状态处理
    if (bIsPressed)
    {
        if (!State.bHolding)
        {
            State.bHolding = true;
            State.StartTime = World->GetTimeSeconds();
            State.bShortValid = true;
        }
        // 阈值检测
        if (State.bShortValid)
        {
            double Elapsed = World->GetTimeSeconds() - State.StartTime;
            if (Elapsed >= ThresholdSeconds)
            {
                State.bShortValid = false;
            }
        }
    }
    else
    {
        // 松开重置
        if (State.bHolding)
        {
            State.bHolding = false;
            State.StartTime = 0.0;
            State.bShortValid = true;
        }
    }

    return State.bShortValid;
}

FHoldPressState* UCommonTools::FindHoldState(UInputAction* Action)
{
    if (!Action) return nullptr;
    return HoldStates.Find(TWeakObjectPtr<UInputAction>(Action));
}
