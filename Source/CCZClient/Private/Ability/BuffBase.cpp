#include "Ability/BuffBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/TempestAttributesComponents.h"
#include "Components/TempestTickingComponent.h"
#include "NiagaraComponent.h"
#include "Objects/TempestBaseAttributeModifier.h"
#include "Objects/TempestBaseAttributeObject.h"

TMap<FString, TWeakObjectPtr<UBuffBase>> UBuffBase::ActivedBuffInstances;

UBuffBase::UBuffBase()
{
    
}

void UBuffBase::TickTempestObject_Implementation(const float Value)
{
	// 如果没有Buff，移除Tick
	if (BuffList.Num() == 0)
	{
		if (UTempestTickingComponent* TickingComp = GetDamageReceiver()->FindComponentByClass<UTempestTickingComponent>())
			TickingComp->RemoveTickableObject(this);
		return;
	}
	
	UpdateBuffState(Value);
	for (FS_Buff& Buff : BuffList)
		if (!Buff.bIsBuffActive && Buff.Duration > 0.0f)
			UpdateEffect(Value, Buff);

}

//累加值时间清零
void UBuffBase::ClearBuffAccumulateValue(FS_Buff& Buff)
{
	Buff.AccumulateTimeHandle = 0.0f;
	Buff.bIsBuffActive= false; 
}

//计算Buff累计值最终结果
float UBuffBase::CalculateAccumulaterValue(const FS_Buff& Buff )
{
    UTempestAttributesComponents* ReceiverAttributesComp = GetDamageReceiver()->FindComponentByClass<UTempestAttributesComponents>();
    for (AActor* Actor : GetDamageDealers())
    {
        UTempestAttributesComponents* DealersAttributesComp = Actor->FindComponentByClass<UTempestAttributesComponents>();
        if (ReceiverAttributesComp && DealersAttributesComp)
        {
            UTempestBaseAttributeObject* ResistanceObj = ReceiverAttributesComp->GetAttributeOfGameplayTag(Buff.ResistanceTag);
            UTempestBaseAttributeObject* EfficiencyObj = DealersAttributesComp->GetAttributeOfGameplayTag(Buff.EfficiencyTag);
            if (ResistanceObj && EfficiencyObj)
            {
                float Resistance = ResistanceObj->AttributeValues.AttributeValue;
                float Efficiency = EfficiencyObj->AttributeValues.AttributeValue;
                float AdjustedValue = Buff.AccumulatedValue * (1.0f + Efficiency) * (1.0f - Resistance);
                return FMath::Max(AdjustedValue, 0.0f); 
            }
        }
    }
    return 1.0f;
}

//检查Buff状态
void UBuffBase::UpdateBuffState(float DeltaTime)
{
    for (FS_Buff& Buff : BuffList)
    {
        //触发频率计算
        Buff.AccumulatedTime += DeltaTime;
        if (Buff.AccumulateTimeHandle <= Buff.MaxAccumulatedTime)
            Buff.AccumulateTimeHandle += DeltaTime;
        //持续减少累计值
        AutoReduceAccumulatedValue(DeltaTime, Buff);
        
        if (Buff.AccumulatedTime >= Buff.AccumulatedFrequency)
        {
            if (Buff.bNeedAccumulate)
            {
                //在持续时间内，且Buff激活状态，且未达到最大累计值
                if (Buff.AccumulateTimeHandle <= Buff.MaxAccumulatedTime &&
                    Buff.bIsBuffActive &&
                    Buff.CurrentAccumulatedValue < Buff.AccumulatedMaxValue)
                {
                    CheckBuffState(Buff , DeltaTime);
                    FString Msg = FString::Printf(TEXT("当前Buff名称：%s, 累计值：%f"), *Buff.BuffName, Buff.CurrentAccumulatedValue);
                    if (GEngine)
                        GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Green, Msg);
                    BuffAccumulatedValue = Buff.CurrentAccumulatedValue;
                }
            }
            //如果Buff不需要累积，直接应用效果
            else
            {
                if (Buff.bIsBuffActive) 
                {
                    ApplyBuffEffect(Buff);
                    ClearBuffAccumulateValue(Buff);
                }
            }
            Buff.AccumulatedTime = 0.0f;
        }
    }
}

void UBuffBase::CallBackBuffApplied(const FString& BuffName)
{
    SpawnBuffStart(BuffName);
}

void UBuffBase::CallBackOnBuffEffectTriggered(FS_Buff Buff)
{
    SpawnBuffEffect(Buff);
}

void UBuffBase::SyncAccumlatedValueToAttributes(const FS_Buff& Buff , float DeltaValue)
{
    if (DeltaValue == 0.0f) return;

    UTempestAttributesComponents* ReceiverAttributesComp = GetDamageReceiver()->FindComponentByClass<UTempestAttributesComponents>();
    if (!ReceiverAttributesComp) return;

    for (const FGameplayTag& Tag : Buff.AccumulationTags)
    {
        UTempestBaseAttributeObject* AttrObj = ReceiverAttributesComp->GetAttributeOfGameplayTag(Tag);
        if (!AttrObj) continue;
        
        float Current = AttrObj->AttributeValues.AttributeValue;
        float MaxVal = AttrObj->AttributeValues.MaxAttributeValue;
        float Target = FMath::Clamp(Current + DeltaValue, 0.0f, MaxVal);

        float ToApply = Target - Current;
        AttrObj->ModifyAttribute(ToApply);
    }
}

void UBuffBase::StartBuff_Implementation()
{
    Super::StartBuff_Implementation();
    OnBuffApplied.AddDynamic(this, &UBuffBase::CallBackBuffApplied);
    OnBuffEffectTriggered.AddDynamic(this, &UBuffBase::CallBackOnBuffEffectTriggered);
    
}

//检查Buff状态
void UBuffBase::CheckBuffState(FS_Buff& Buff, float DeltaTime)
{
    //检查是否达到最大累计值
    if (Buff.CurrentAccumulatedValue >= Buff.AccumulatedMaxValue) return;
    //检查是否在持续时间内，是否处于Buff效果触发阶段
    if (Buff.AccumulateTimeHandle <= Buff.MaxAccumulatedTime && Buff.bIsBuffActive)
    {
        if (!Buff.bIsResistanceCalculated)
        {
            Buff.AccumulatedValue = CalculateAccumulaterValue(Buff);
            Buff.bIsResistanceCalculated = true;
        }
        Buff.CurrentAccumulatedValue += Buff.AccumulatedValue;
        SyncAccumlatedValueToAttributes(Buff,Buff.AccumulatedValue);
        OnBuffAccumulatedValueChanged.Broadcast(Buff.BuffName, Buff.CurrentAccumulatedValue,Buff.AccumulatedMaxValue);
        if (Buff.CurrentAccumulatedValue >= Buff.AccumulatedMaxValue)
        {
            // 如果已经在效果阶段，重新触发效果并重置持续时间
            if (Buff.bIsInEffectPhase)
            {
                // 重置持续时间
                if (FS_Buff* OriginalData = BuffDataTable->FindRow<FS_Buff>(*Buff.BuffName, TEXT("")))
                    Buff.Duration = OriginalData->Duration;
                Buff.bIsBuffActive = false; 
                Buff.BuffTimeHandle = 0.0f;
                Buff.bIsBuffActive = false;
                
                OnBuffEffectTriggered.Broadcast(Buff);
            }
            else
            {
                // 第一次达到最大值，进入效果阶段
                ApplyBuffEffect(Buff);
                ClearBuffAccumulateValue(Buff);
                
            }
        }
    }
}

//Buff入口，从能力启用开始
void UBuffBase::AddBuff(const  FString& BuffName,ESocketName ReceiverSocketType , ESocketName DealerSocketType )
{
    if (!BuffDataTable) return;
    FS_Buff* Row = BuffDataTable->FindRow<FS_Buff>(*BuffName, TEXT(""));
    if (!Row) return;
    UTempestTickingComponent* TickingComp = GetDamageReceiver()->FindComponentByClass<UTempestTickingComponent>();
    if (!TickingComp) return;
    FString UniqueKey = GenerateUniqueBuffKey(BuffName);
    BuffSocketMap.Add(UniqueKey + TEXT("_Receiver"), ReceiverSocketType);
    BuffSocketMap.Add(UniqueKey + TEXT("_Dealer"), DealerSocketType);
    
    if (TWeakObjectPtr<UBuffBase>* ExistingPtr = ActivedBuffInstances.Find(UniqueKey))
    {
        if (ExistingPtr->IsValid())
        {
            UBuffBase* ExistingBuff = ExistingPtr->Get();
            if (!ExistingBuff || ExistingBuff->IsUnreachable() || ExistingBuff->HasAnyFlags(RF_BeginDestroyed))
                ActivedBuffInstances.Remove(UniqueKey);
            else
                for (FS_Buff& Buff : ExistingBuff->BuffList)
                    if (!Buff.BuffName.IsEmpty() && Buff.BuffName == BuffName)
                    {
                        ExistingBuff->ReactivateExistingBuff(Buff);
                        return;
                    }
        }
        else
            ActivedBuffInstances.Remove(UniqueKey);
    }

    // 创建新 Buff
    CreateNewBuffAndCache(UniqueKey,BuffName, *Row, TickingComp);
}


FString UBuffBase::GenerateUniqueBuffKey(const FString& BuffName)
{
    FString ReceiverName = GetDamageReceiver() ? GetDamageReceiver()->GetName() : TEXT("None");
    FString DealerName = TEXT("None");
    TArray<AActor*> Dealers = GetDamageDealers();
    if (Dealers.Num() > 0 && Dealers[0])
        DealerName = Dealers[0]->GetName();
        
    return FString::Printf(TEXT("%s_%s_%s"), *BuffName, *ReceiverName, *DealerName);
}


//重新激活现有的Buff
void UBuffBase::ReactivateExistingBuff(FS_Buff& Buff)
{
    // 如果 Buff 已经结束，检查累积值再决定是否重新激活
    if (Buff.Duration <= 0.0f)
    {
        // 对于需要累积值的Buff，只有累积值未达到最大值才能重新激活
        if (Buff.bNeedAccumulate && Buff.CurrentAccumulatedValue >= Buff.AccumulatedMaxValue)
        {
            FString BlockMsg = FString::Printf(TEXT("阻止重新激活已结束的Buff - %s 积累值已满 (%.1f/%.1f)"), 
                *Buff.BuffName, Buff.CurrentAccumulatedValue, Buff.AccumulatedMaxValue);
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(24, 2.0f, FColor::Red, BlockMsg);
            return;
        }

        // 重置为初始状态
        if (FS_Buff* OriginalData = BuffDataTable->FindRow<FS_Buff>(*Buff.BuffName, TEXT("")))
        {
            Buff.Duration = OriginalData->Duration;
        }

        Buff.bIsInEffectPhase = false;
        Buff.CurrentAccumulatedValue = 0.0f;
        Buff.AccumulateTimeHandle = 0.0f;
        Buff.bIsBuffActive = true;
        Buff.bIsResistanceCalculated = false;
        Buff.AccumulatedTime = 0.0f;
        Buff.BuffTimeHandle = 0.0f;
        Buff.EffectPhaseTimeHandle = 0.0f;

        OnBuffApplied.Broadcast(Buff.BuffName);
        FS_Buff BuffCopy = Buff;
        OnBuffCreateInfo.Broadcast(this,BuffCopy);
        FString RestartMsg = FString::Printf(TEXT("重新激活已结束的Buff - %s"), *Buff.BuffName);
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(17, 2.0f, FColor::Blue, RestartMsg);
        return;
    }

    // 如果 Buff 仍在效果阶段且有剩余持续时间
    if (Buff.bIsInEffectPhase && Buff.Duration > 0.0f)
    {
        // 对于不需要积累值的 Buff，延长持续时间
        if (!Buff.bNeedAccumulate)
        {
            if (FS_Buff* OriginalData = BuffDataTable->FindRow<FS_Buff>(*Buff.BuffName, TEXT("")))
            {
                Buff.Duration = FMath::Max(Buff.Duration, OriginalData->Duration);
            }

            FString ExtendMsg = FString::Printf(TEXT("延长无积累值Buff持续时间 - %s"), *Buff.BuffName);
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(18, 2.0f, FColor::Orange, ExtendMsg);
        }
        
        // 对于需要积累值的 Buff，只有当前积累值小于最大值时才能重新开始累积
        else if (Buff.CurrentAccumulatedValue < Buff.AccumulatedMaxValue)
        {
            Buff.bIsInEffectPhase = false;
            Buff.AccumulateTimeHandle = 0.0f;
            Buff.bIsBuffActive = true;
            Buff.bIsResistanceCalculated = false;
            Buff.AccumulatedTime = 0.0f;

            OnBuffApplied.Broadcast(Buff.BuffName);

            FString RestartAccumulateMsg = FString::Printf(TEXT("重新开始积累 - %s (当前积累值: %.1f/%.1f)"), 
                *Buff.BuffName, Buff.CurrentAccumulatedValue, Buff.AccumulatedMaxValue);
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(19, 2.0f, FColor::Cyan, RestartAccumulateMsg);
        }
        else
        {
            FString BlockMsg = FString::Printf(TEXT("阻止重新激活 - Buff %s 积累值已满 (%.1f/%.1f)"), 
                *Buff.BuffName, Buff.CurrentAccumulatedValue, Buff.AccumulatedMaxValue);
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(20, 2.0f, FColor::Yellow, BlockMsg);
        }
        return;
    }

    // 如果 Buff 不在效果阶段，正常重新激活
    Buff.AccumulateTimeHandle = 0.0f;
    Buff.bIsBuffActive = true;
    Buff.bIsResistanceCalculated = false;
    Buff.AccumulatedTime = 0.0f;

    if (Buff.bNeedAccumulate)
        Buff.CurrentAccumulatedValue = 0.0f;

    OnBuffApplied.Broadcast(Buff.BuffName);
    FString ReactiveMsg = FString::Printf(TEXT("正常重新激活 - %s"), *Buff.BuffName);
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(21, 2.0f, FColor::Green, ReactiveMsg);
}

void UBuffBase::CreateNewBuffAndCache(const FString& UniqueKey,const FString& BuffName, const FS_Buff& BuffData, UTempestTickingComponent* TickingComp)
{
    FName BuffNameKey = FName(*BuffName);
    UBuffBase* BuffObj = nullptr;
    if (TSubclassOf<UBuffBase>* BuffClass = BuffNameToClass.Find(BuffNameKey))
        BuffObj = NewObject<UBuffBase>(GetDamageReceiver(), *BuffClass);
    else
        BuffObj = NewObject<UBuffBase>(GetDamageReceiver(), this->GetClass());

    if (BuffObj)
    {
        // 初始化Buff实例
        BuffObj->BuffDataTable = BuffDataTable;
        BuffObj->BuffNameToClass = BuffNameToClass;
        BuffObj->SetDamageReceiver(GetDamageReceiver());
        BuffObj->BuffSocketMap = BuffSocketMap;
        TArray<AActor*> Dealers = GetDamageDealers();
        for (AActor* Dealer : Dealers)
            BuffObj->SetDamageDealer(Dealer);
        
        // 添加Buff数据
        FS_Buff NewBuffData = BuffData;
        NewBuffData.bIsBuffActive = true;
        BuffObj->BuffList.Add(NewBuffData);
        FS_Buff CreatedBuff = BuffObj->BuffList.Last();
        
        ActivedBuffInstances.Add(UniqueKey, BuffObj);
        BuffObj->StartBuff_Implementation();
        BuffObj->StartBuff();
        TickingComp->AddTickableObject(BuffObj);
        OnBuffApplied.Broadcast(BuffName);
        OnBuffCreateInfo.Broadcast(BuffObj,CreatedBuff);
    }
}


UBuffBase* UBuffBase::GetActiveBuffInstance(const FString& BuffName)
{
    for (auto& Pair : ActivedBuffInstances)
        if (Pair.Value.IsValid())
        {
            UBuffBase* BuffInstance = Pair.Value.Get();
            for (const FS_Buff& Buff : BuffInstance->BuffList)
                if (Buff.BuffName == BuffName)
                    return BuffInstance;
        }
    return nullptr;
}


// 更新Buff状态
void UBuffBase::UpdateEffect_Implementation(float DeltaTime, FS_Buff& Buff)
{
    
    //销毁粒子
    if (Buff.BuffTimeHandle >= Buff.Duration - 0.1f)
    {
        for (UNiagaraComponent* Comp : StartNiagaraComponents)
        {
            if (Comp && IsValid(Comp))
                Comp->DestroyComponent();
        }
        StartNiagaraComponents.Empty();
        
        if (EffectNiagaraComponent && IsValid(EffectNiagaraComponent))
        {
            EffectNiagaraComponent->DestroyComponent();
            EffectNiagaraComponent = nullptr;
        }
        
    }
    //子类重写
}

//Buff粒子效果
void UBuffBase::SpawnBuffEffect_Implementation(FS_Buff& Buff)
{
    if (!BuffEffectNiagara) return;
    AActor* Owner = GetDamageReceiver();
    if (!Owner) return;

    if (EffectNiagaraComponent && IsValid(EffectNiagaraComponent))
    {
        EffectNiagaraComponent->DestroyComponent();
        EffectNiagaraComponent = nullptr;
    }

    FString BuffKey = GenerateUniqueBuffKey(Buff.BuffName) + TEXT("_Receiver");
    ESocketName SocketType = ESocketName::Root;
    if (BuffSocketMap.Contains(BuffKey)) {
        if (ESocketName* FoundSocket = BuffSocketMap.Find(BuffKey)) {
            SocketType = *FoundSocket;
        }
    }
    FName SocketName = UCommonTools::GetSocketNameFromType(SocketType);
    if (!UCommonTools::HasSocket(Owner, SocketName))
        SocketName = NAME_Root;
    USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();
    if (!MeshComp) return;

    EffectNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
        BuffEffectNiagara,
        MeshComp,
        SocketName,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget,
        true
    );

    EffectNiagaraComponent->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
}


void UBuffBase::SpawnBuffStart_Implementation(const FString& BuffName)
{
    if (!BuffStartNiagara) return;

    for (UNiagaraComponent* Comp : StartNiagaraComponents)
        if (Comp && IsValid(Comp))
            Comp->DestroyComponent();
    StartNiagaraComponents.Empty();

    FString BuffKey = GenerateUniqueBuffKey(BuffName) + TEXT("_Dealer");
    ESocketName SocketType = ESocketName::Root;
    if (BuffSocketMap.Contains(BuffKey)) {
        if (ESocketName* FoundSocket = BuffSocketMap.Find(BuffKey)) {
            SocketType = *FoundSocket;
        }
    }
    FName SocketName = UCommonTools::GetSocketNameFromType(SocketType);

    TArray<AActor*> Dealers = GetDamageDealers();
    for (AActor* Owner : Dealers)
    {
        if (!Owner) continue;
        USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();
        if (!MeshComp) continue;
        if (!UCommonTools::HasSocket(Owner, SocketName))
            SocketName = NAME_Root;
        UNiagaraComponent* Comp = UNiagaraFunctionLibrary::SpawnSystemAttached(
            BuffStartNiagara,
            MeshComp,
            SocketName,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );

        if (Comp && IsValid(Comp))
        {
            Comp->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
            StartNiagaraComponents.Add(Comp);
        }
    }
}


void UBuffBase::ApplyBuffEffect_Implementation(FS_Buff& Buff)
{
    // 进入效果阶段
    Buff.bIsInEffectPhase = true;
    Buff.bIsBuffActive = false;  
    
    FName BuffNameKey = FName(*Buff.BuffName);
    UBuffBase* BuffObj = nullptr;

    // 查找对应的 Buff 类
    if (TSubclassOf<UBuffBase>* BuffClass = BuffNameToClass.Find(BuffNameKey))
        if (*BuffClass) 
            BuffObj = NewObject<UBuffBase>(GetDamageReceiver(), *BuffClass);
    else
        BuffObj = NewObject<UBuffBase>(GetDamageReceiver(), this->GetClass());
    if (BuffObj)
        BuffObj->OnBuffEffectTriggered.Broadcast(Buff);
}

//固定时间减少积累值
void UBuffBase::AutoReduceAccumulatedValue(float DeltaTime, FS_Buff& Buff)
{
	if (Buff.ReduceFrequency <= 0.0f || Buff.ReduceValue <= 0.0f) return;
	Buff.ReduceTimeHandle += DeltaTime;
	if (Buff.ReduceTimeHandle >= Buff.ReduceFrequency)
	{
		Buff.CurrentAccumulatedValue = FMath::Max(Buff.CurrentAccumulatedValue - Buff.ReduceValue, 0.0f);
		Buff.ReduceTimeHandle = 0.0f;
		FString Msg = FString::Printf(TEXT("自动减少后 - Buff名称：%s, 累计值：%f"), *Buff.BuffName, Buff.CurrentAccumulatedValue);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, Msg);
	}
}


void UBuffBase::ClearAllBuffsForActor(AActor* TargetActor)
{
    if (!TargetActor || !IsValid(TargetActor)) return;

    FString TargetActorName = TargetActor->GetName();
    TArray<FString> KeysToRemove;

    for (auto& Pair : ActivedBuffInstances)
    {
        if (Pair.Value.IsValid() && Pair.Key.Contains(TargetActorName))
        {
            UBuffBase* BuffInstance = Pair.Value.Get();
            if (BuffInstance)
            {
                BuffInstance->BuffList.Empty();
                
                for (UNiagaraComponent* Comp : BuffInstance->StartNiagaraComponents)
                {
                    if (Comp && IsValid(Comp))
                        Comp->DestroyComponent();
                }
                BuffInstance->StartNiagaraComponents.Empty();

                if (BuffInstance->EffectNiagaraComponent && IsValid(BuffInstance->EffectNiagaraComponent))
                {
                    BuffInstance->EffectNiagaraComponent->DestroyComponent();
                    BuffInstance->EffectNiagaraComponent = nullptr;
                }

                if (UTempestTickingComponent* TickingComp = TargetActor->FindComponentByClass<UTempestTickingComponent>())
                    TickingComp->RemoveTickableObject(BuffInstance);
            }
            KeysToRemove.Add(Pair.Key);
        }
    }
    
    for (const FString& Key : KeysToRemove)
        ActivedBuffInstances.Remove(Key);

    FString ClearMsg = FString::Printf(TEXT("已清空Actor %s 的所有Buff实例"), *TargetActorName);
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(22, 3.0f, FColor::Orange, ClearMsg);
}

void UBuffBase::ClearSpecificBuff(const FString& BuffName)
{
    TArray<FString> KeysToRemove;
    for (auto& Pair : ActivedBuffInstances)
        if (Pair.Value.IsValid())
        {
            UBuffBase* BuffInstance = Pair.Value.Get();
            if (BuffInstance)
                BuffInstance->BuffList.RemoveAll([&BuffName](const FS_Buff& Buff) {
                    return Buff.BuffName == BuffName;
                });
        }
    
    for (const FString& Key : KeysToRemove)
        ActivedBuffInstances.Remove(Key);

    FString ClearMsg = FString::Printf(TEXT("已清空指定Buff: %s"), *BuffName);
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(23, 3.0f, FColor::Yellow, ClearMsg);
}