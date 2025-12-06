// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/DeBuff.h"
#include "CCZClient/Public/Ability/BuffBase.h"
#include "Components/TempestAttributesComponents.h"
#include "Objects/TempestBaseAttributeObject.h"


//重写应用Buff效果（触发单次）
void UDeBuff::ApplyBuffEffect_Implementation(FS_Buff& Buff)
{
    if (IsSupportedBuffName(Buff.BuffName))
    {
        Super::ApplyBuffEffect_Implementation(Buff);
        OnBuffEffectTriggered.Broadcast(Buff);
    }
}

//重写Buff持续影响效果（对应频率次）
void UDeBuff::UpdateEffect_Implementation(float DeltaTime, FS_Buff& Buff)
{
    Super::UpdateEffect_Implementation(DeltaTime, Buff);
    UTempestAttributesComponents* AttributesComp = GetDamageReceiver()->FindComponentByClass<UTempestAttributesComponents>();
    if (!AttributesComp) return;
    
    if (!IsSupportedBuffName(Buff.BuffName)) return;
    
    if (Buff.Duration > 0.0f)
    {
        Buff.BuffTimeHandle += DeltaTime;
        if (Buff.BuffTimeHandle >= Buff.EffectFrequency)
        {
            // 遍历所有需要修改的属性Tag
            for (const FGameplayTag& Effect : Buff.EffectTags)
            {
                UTempestBaseAttributeObject* AttributeObject = AttributesComp->GetAttributeOfGameplayTag(Effect);
                if (AttributeObject)
                {
                    float CurrentAttribute = AttributeObject->AttributeValues.AttributeValue;
                    float MaxAttribute = AttributeObject->AttributeValues.MaxAttributeValue;
                    
                    // 检查当前属性值是否有效
                    if (CurrentAttribute > 0.0f)
                    {
                        // 使用可重写的伤害计算函数
                        DamageValue = CalculateDamageValue(MaxAttribute, Buff.EffectFrequency, Buff);
                        AttributeObject->ModifyAttribute(-DamageValue); 
                    }
                }
            }
            
            Buff.Duration -= Buff.EffectFrequency;
            Buff.BuffTimeHandle = 0.0f;
        }
    }
    else if (Buff.Duration > -1.0f)
        Buff.Duration -= DeltaTime;

}

//检查是否有该Buff及名称
bool UDeBuff::IsSupportedBuffName(const FString& BuffName) const
{
    FName BuffNameKey = FName(*BuffName);
    return BuffNameToClass.Contains(BuffNameKey);
}

//伤害计算
float UDeBuff::CalculateDamageValue_Implementation(float MaxHp, float EffectFrequency, FS_Buff& Buff)
{
    return MaxHp * DamageRatio * EffectFrequency;
}


