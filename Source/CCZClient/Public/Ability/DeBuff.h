// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffBase.h"
#include "DeBuff.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CCZCLIENT_API UDeBuff : public UBuffBase
{
	GENERATED_BODY()

public:

	
	
	//重写父类实现
	virtual void ApplyBuffEffect_Implementation(FS_Buff& Buff) override;
	virtual void UpdateEffect_Implementation(float DeltaTime, FS_Buff& Buff) override;


protected:

	// 检查是否支持该Buff名称
	UFUNCTION(BlueprintCallable, Category = "Buff")
	virtual bool IsSupportedBuffName(const FString& BuffName) const;

	// 计算伤害值（蓝图可重写）
	UFUNCTION(BlueprintNativeEvent, Category = "Buff")
	float CalculateDamageValue(float MaxHp, float EffectFrequency, FS_Buff& Buff);
	virtual float CalculateDamageValue_Implementation(float MaxHp, float EffectFrequency, FS_Buff& Buff);
};
