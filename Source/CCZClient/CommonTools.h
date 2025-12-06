// Fill out your copyright notice in the Description page of Project Settings.

// CommonToolsSubsystem.h
#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Subsystems/WorldSubsystem.h"
#include "CommonTools.generated.h"

UENUM(BlueprintType)
enum class ESocketName : uint8
{
    Root        UMETA(DisplayName = "Root"),
    Spine       UMETA(DisplayName = "Spine"), 
    Head        UMETA(DisplayName = "Head"),
    HandLeft    UMETA(DisplayName = "Hand Left"),
    HandRight   UMETA(DisplayName = "Hand Right"),
    FootLeft    UMETA(DisplayName = "Foot Left"),
    FootRight   UMETA(DisplayName = "Foot Right"),
    Custom      UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EImpactDirection : uint8
{
    None  UMETA(DisplayName = "None"),
    Front UMETA(DisplayName = "Front"),
    Back  UMETA(DisplayName = "Back"),
    Right UMETA(DisplayName = "Right"),
    Left  UMETA(DisplayName = "Left"),
};

USTRUCT()
struct FHoldPressState
{
    GENERATED_BODY()
    double StartTime = 0.0;
    bool   bHolding = false;
    bool   bShortValid = true;
};

UCLASS()
class CCZCLIENT_API UCommonTools : public UWorldSubsystem

{
    GENERATED_BODY()

public:
    
    // 获取子系统
    UFUNCTION(BlueprintCallable, Category="CommonTools", meta=(WorldContext="WorldContextObject"))
    static UCommonTools* GetCommonToolsSubsystem(UObject* WorldContextObject);

    // 根据标签获取角色列表
    UFUNCTION(BlueprintPure)
    const TArray<AActor*>& GetCharactersByTag(const FName& Tag);

    // 将角色添加到标签映射
    UFUNCTION(BlueprintCallable)
    void AddCharacterToMap(AActor* Character, const FName& Tag);

    // 根据Yaw角度获取方向字符串
    UFUNCTION(BlueprintPure, Category="Math")
    static FString GetDirection(float Yaw, const TArray<EImpactDirection>& IgnoreDirections);

    // 检查角色是否具有指定的Socket
    UFUNCTION(BlueprintPure, Category="Socket Utils")
    static bool HasSocket(AActor* Actor, FName SocketName);

    // 根据Socket类型获取Socket名称
    UFUNCTION(BlueprintPure, Category="Socket Utils")
    static FName GetSocketNameFromType(ESocketName SocketType);

    //受击方向计算
    UFUNCTION(BlueprintCallable, Category="Socket Utils")
    EImpactDirection ImpactDirection(FVector DamageDealerVector,
                                            FVector DamageReceiverVector,
                                            float FrontHalfAngleDeg = 30.f,
                                            float RightHalfAngleDeg = 120.f,
                                            float BackHalfAngleDeg = 30.f,
                                            FVector ReceiverForward = FVector(1.f, 0.f, 0.f));

    UFUNCTION(BlueprintCallable,Category = "Direction")
    static EImpactDirection GetDirectionByNearly(float Yaw, const TArray<EImpactDirection>& IgnoreDirections);
    
    //面对敌人
    UFUNCTION(BlueprintCallable, Category="CommonTools")
    static bool ForceFaceTarget(AActor* Owner, AActor* Attacker, bool bInstant = true, float InterpSpeed = 10.f, bool bSyncController = true, float TempRotateDuration = 0.f);

    //转向方向
    UFUNCTION(BlueprintCallable, Category="CommonTools")
    static bool ForceFaceTargetByDirection(AActor* Owner,
                                AActor* Attacker,
                                const FString& Direction,
                                bool bInstant = true,
                                float InterpSpeed = 10.f,
                                bool bSyncController = true,
                                float TempRotateDuration = 0.f);

    
    UFUNCTION(BlueprintCallable, Category="CommonTools")
    static void DestroyComponentManual(UObject* Component);
    
    //按键检测、长按判定
    UFUNCTION(BlueprintCallable, Category="CommonTools")
    bool UpdatePressCondition(UInputAction* Action, float ThresholdSeconds, bool bIsPressed);
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="CommonTools")
    TArray<EImpactDirection>IgnoreDirections;

    
    
private:
    TMap<FName, TArray<AActor*>> CharacterMap;

    TMap<TWeakObjectPtr<UInputAction>, FHoldPressState> HoldStates;
    FHoldPressState* FindHoldState(UInputAction* Action);
};