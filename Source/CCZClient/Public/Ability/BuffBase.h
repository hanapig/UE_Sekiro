// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseBuffObject.h"
#include "Engine/DataTable.h"
#include "GameplayTags.h"
#include "CCZClient/CommonTools.h"
#include "Components/ActorComponent.h"
#include "BuffBase.generated.h"

class UTempestTickingComponent;
class UNiagaraComponent;
class UNiagaraSystem;

//Buff的结构体
USTRUCT(BlueprintType)
struct FS_Buff : public FTableRowBase
{
	GENERATED_BODY()
	
	// Buff名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	FString BuffName = "None";
	
	// Buff每次累计的数值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	float AccumulatedValue = 20.0f;

	// Buff最大累计值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	float AccumulatedMaxValue = 100.0f;

	// Buff异常状态值满值后造成状态效果的时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	float Duration = 5.0f;

	//累加值累加的频率
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	float AccumulatedFrequency = 1.0f;

	//累加值满后效果的触发频率
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	float EffectFrequency = 1.0f;

	//Buff是否产生虚血
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	bool bIsVirtualHealth = false;

	//积累值的最大持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	float MaxAccumulatedTime = 10.0f;

	//积累值减少的频率
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	float ReduceFrequency = 2.0f;

	//积累值减少值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	float ReduceValue = 1.0f;

	//是否需要累计值
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Buff")
	bool bNeedAccumulate = true;

	//Buff受影响的抗性属性Tag
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Buff")
	FGameplayTag ResistanceTag;

	//Buff影响属性
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Buff")
	TArray<FGameplayTag> EffectTags;

	//Buff累计值同步对应tag属性
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Buff")
	TArray<FGameplayTag> AccumulationTags;

	//Buff受影响的抗性属性Tag
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Buff")
	FGameplayTag EfficiencyTag;
	
	//控制计时器
	UPROPERTY()
	float AccumulateTimeHandle = 0.0f;

	//Buff计时器
	UPROPERTY()
	float BuffTimeHandle = 0.0f;
	
	//Buff累加计时器
	UPROPERTY()
	float AccumulatedTime = 0.0f;

	//抗性计算标识符
	UPROPERTY()
	bool bIsResistanceCalculated = false;

	//减少累计值计时器
	UPROPERTY()
	float ReduceTimeHandle = 0.0f;

	//Buff是否在效果阶段
	UPROPERTY()
	bool bIsInEffectPhase = false;

	// Buff初始累计值
	UPROPERTY()
	float CurrentAccumulatedValue = 0.0f;

	// Buff是否处于开始累计状态
	UPROPERTY()
	bool bIsBuffActive = true;

	// 效果阶段计时
	UPROPERTY()
	float EffectPhaseTimeHandle = 0.0f;  
};

// 累计值更新
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBuffAccumulatedValueChanged, const FString&, BuffName, float, CurrentValue, float, MaxValue);
// Buff施加
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuffApplied, const FString&, BuffName);
// Buff效果触发
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuffEffectTriggered , FS_Buff, Buff);
//Buff创建
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuffCreateInfo , UBuffBase*, BuffObj, FS_Buff, BuffData);

UCLASS()
class CCZCLIENT_API UBuffBase : public UTempestBaseBuffObject 
{
	GENERATED_BODY()

	
public:
	
	virtual void TickTempestObject_Implementation(const float Value) override;
	
	UBuffBase();

	// Buff累计值
	UPROPERTY()
	float BuffAccumulatedValue = 0.0f;

	//实际伤害
	UPROPERTY()
	float DamageValue = 0.0f;
	
	//Buff数组
	UPROPERTY()
	TArray<FS_Buff>BuffList;

	// Buff触发时粒子效果组件
	UPROPERTY()
	UNiagaraComponent* EffectNiagaraComponent;

	// Buff开始时粒子效果组件
	UPROPERTY()
	TArray<UNiagaraComponent*> StartNiagaraComponents;

	// 广播声明
	UPROPERTY(BlueprintAssignable, Category="Buff Events")
	FOnBuffAccumulatedValueChanged OnBuffAccumulatedValueChanged;
	UPROPERTY(BlueprintAssignable, Category="Buff Events")
	FOnBuffApplied OnBuffApplied;
	UPROPERTY(BlueprintAssignable, Category="Buff Events")
	FOnBuffEffectTriggered OnBuffEffectTriggered;
	UPROPERTY(BlueprintAssignable, Category="Buff Events")
	FOnBuffCreateInfo OnBuffCreateInfo;
	
	// 可在蓝图中配置的伤害系数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff Config")
	float DamageRatio = 0.025f;
	
	//Buff管理
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	TMap<FName, TSubclassOf<UBuffBase>> BuffNameToClass;
	
	//数据表格配置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	UDataTable* BuffDataTable;
	
	//Buff粒子效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	UNiagaraSystem*  BuffEffectNiagara;
	
	//Buff粒子效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buff")
	UNiagaraSystem*  BuffStartNiagara;
	
	UFUNCTION()
	void CallBackBuffApplied(const FString& BuffName);
	
	UFUNCTION()
	void CallBackOnBuffEffectTriggered(FS_Buff Buff);

	UFUNCTION()
	void SyncAccumlatedValueToAttributes(const FS_Buff& Buff , float DeltaValue);
	
	//更新Buff累计值
	UFUNCTION(BlueprintCallable, Category="Buff")
	void CheckBuffState(FS_Buff& Buff ,float DeltaTime);

	//清空累计值
	UFUNCTION(BlueprintCallable, Category="Buff")
	void ClearBuffAccumulateValue(FS_Buff& Buff);

	UFUNCTION(BlueprintNativeEvent, Category = "Buff")
	void ApplyBuffEffect(FS_Buff& Buff);
	virtual void ApplyBuffEffect_Implementation(FS_Buff& Buff);
	UFUNCTION(BlueprintNativeEvent, Category = "Buff")
	void UpdateEffect(float DeltaTime, FS_Buff& Buff);
	virtual void UpdateEffect_Implementation(float DeltaTime, FS_Buff& Buff);

	//计算累计值
	UFUNCTION(BlueprintCallable, Category="Buff")
	float CalculateAccumulaterValue(const FS_Buff& Buff);

	//Tick更新Buff累计值 
	UFUNCTION(BlueprintCallable, Category="Buff")
	void UpdateBuffState(float DeltaTime);

	//施加Buff对外接口
	UFUNCTION(BlueprintCallable, Category="Buff")
	void AddBuff(const FString& BuffName,ESocketName ReceiverSocketType = ESocketName::Root, ESocketName DealerSocketType = ESocketName::Root);

	//自动减少积累值
	UFUNCTION(BlueprintCallable, Category="Buff")
	void AutoReduceAccumulatedValue(float DeltaTime, FS_Buff& Buff);

	//生成积累值满后的造成粒子
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Buff")
	void SpawnBuffEffect(FS_Buff& Buff);
	virtual void SpawnBuffEffect_Implementation(FS_Buff& Buff);
	
	//生成刚开始积累的粒子
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Buff")
	void SpawnBuffStart(const FString& BuffName);
	virtual void SpawnBuffStart_Implementation(const FString& BuffName);

	// 在BuffBase.h中添加声明
	UFUNCTION(BlueprintCallable, Category="Buff Management")
	void ClearAllBuffsForActor(AActor* TargetActor);
	
	    // 清空指定名称的Buff
	UFUNCTION(BlueprintCallable, Category="Buff Management")
	void ClearSpecificBuff(const FString& BuffName);
	
	// 实例化Buff对象
	UFUNCTION()
	void ReactivateExistingBuff(FS_Buff& Buff);
	UFUNCTION()
	void CreateNewBuffAndCache(const FString& UniqueKey,const FString& BuffName, const FS_Buff& BuffData, UTempestTickingComponent* TickingComp);
	UFUNCTION()
	FString GenerateUniqueBuffKey(const FString& BuffName);

	// 获取当前激活的Buff实例
	UFUNCTION(BlueprintCallable, Category = "Buff System")
	static UBuffBase* GetActiveBuffInstance(const FString& BuffName);

	virtual void StartBuff_Implementation() override;

private:
    static TMap<FString, TWeakObjectPtr<UBuffBase>> ActivedBuffInstances;

	UPROPERTY()
	TMap<FString,ESocketName>BuffSocketMap;
};
