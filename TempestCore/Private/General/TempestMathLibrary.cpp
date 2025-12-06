// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved.

#include "General/TempestMathLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"

// 新增：CustomThunk/反射所需
#include "UObject/Stack.h"
#include "UObject/UnrealType.h"

float UTempestMathLibrary::GetDistanceBetweenActors(class AActor* From, class AActor* To)
{
    if (From && To)
    {
        return FVector::Dist(From->GetActorLocation(), To->GetActorLocation());
    }
    return 0.f;
}

FRotator UTempestMathLibrary::GetAnglesBetweenActors(class AActor* From, class AActor* To)
{
    FRotator LocalRotation(0.f, 0.f, 0.f);
    if (From && To)
    {
        FRotator LocalFromRotation = From->GetActorRotation();
        FRotator LocalToRotation = UKismetMathLibrary::FindLookAtRotation(From->GetActorLocation(), To->GetActorLocation());
        FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(LocalFromRotation, LocalToRotation);
        LocalRotation = DeltaRotation;
    }
    return LocalRotation;
}

void UTempestMathLibrary::SortActorsByDistanceAndReferencePoint(TArray<class AActor*>& SortedActors, const TArray<class AActor*>& ArrayToSort, const FVector& ReferencePoint)
{
    // Copy the array to be sorted
    SortedActors = ArrayToSort;

    // Sort the copied array
    for (int32 i = 0; i < SortedActors.Num() - 1; ++i)
    {
        for (int32 j = 0; j < SortedActors.Num() - i - 1; ++j)
        {
            if (!CompareActorsByDistance(SortedActors[j], SortedActors[j + 1], ReferencePoint))
            {
                // Swap elements
                AActor* Temp = SortedActors[j];
                SortedActors[j] = SortedActors[j + 1];
                SortedActors[j + 1] = Temp;
            }
        }
    }
}

void UTempestMathLibrary::SortArrayByValue(TArray<float>& SortedArray, const TArray<float>& ArrayToSort)
{
    // Copy the array to be sorted
    SortedArray = ArrayToSort;

    // Sort the copied array
    SortedArray.Sort();
}

class AActor* UTempestMathLibrary::GetClosestActorToCameraCenter(class UCameraComponent* CameraComponent, const TArray<class AActor*>& Actors)
{
    if (!CameraComponent || Actors.Num() == 0)
    {
        return nullptr;
    }

    // Get the camera's forward vector and position
    FVector CameraForward = CameraComponent->GetForwardVector();
    FVector CameraLocation = CameraComponent->GetComponentLocation();

    // Track the closest actor and smallest angle
    AActor* ClosestActor = nullptr;
    float SmallestAngle = FLT_MAX;

    for (AActor* Actor : Actors)
    {
        if (Actor)
        {
            // Calculate the direction from the camera to the actor
            FVector DirectionToActor = (Actor->GetActorLocation() - CameraLocation).GetSafeNormal();
            float Angle = FMath::Acos(FVector::DotProduct(CameraForward, DirectionToActor));

            // Check if this angle is the smallest we've found
            if (Angle < SmallestAngle)
            {
                SmallestAngle = Angle;
                ClosestActor = Actor;
            }
        }
    }

    return ClosestActor;
}

float UTempestMathLibrary::GetCameraAngleToActor(class UCameraComponent* CameraComponent, const class AActor* InActor)
{
    if (!CameraComponent || !InActor)
    {
        return -1.f;
    }

    // Get the camera's forward vector and position
    FVector CameraForward = CameraComponent->GetForwardVector();
    FVector CameraLocation = CameraComponent->GetComponentLocation();

    // Calculate the direction from the camera to the actor
    FVector DirectionToActor = (InActor->GetActorLocation() - CameraLocation).GetSafeNormal();
    float Angle = FMath::Acos(FVector::DotProduct(CameraForward, DirectionToActor));

    return Angle;
}

bool UTempestMathLibrary::CompareActorsByDistance(const class AActor* A, const class AActor* B, const FVector& ReferencePoint)
{
    // Compare the squared distances of the actors from the reference point
    float DistA = (A && A->IsValidLowLevel()) ? (A->GetActorLocation() - ReferencePoint).SizeSquared() : FLT_MAX;
    float DistB = (B && B->IsValidLowLevel()) ? (B->GetActorLocation() - ReferencePoint).SizeSquared() : FLT_MAX;
    return DistA < DistB;
}

DEFINE_FUNCTION(UTempestMathLibrary::execSortWeaponArryByLvIdDesc)
{
    Stack.MostRecentProperty = nullptr;

    // 参数1：数组（by-ref）-> 传 nullptr，从 Stack 上拿地址与属性
    Stack.StepCompiledIn<FArrayProperty>(nullptr);
    void* ArrayAddr = Stack.MostRecentPropertyAddress;
    FArrayProperty* ArrayProp = CastField<FArrayProperty>(Stack.MostRecentProperty);

    // 参数2：firstID（by-value）-> 必须提供接收地址
    int32 FirstID = 0;
    Stack.StepCompiledIn<FIntProperty>(&FirstID);

    // 参数3：secondID（by-value）
    int32 SecondID = 0;
    Stack.StepCompiledIn<FIntProperty>(&SecondID);

    P_FINISH;

    if (!ArrayAddr || !ArrayProp)
    {
        return;
    }

    P_NATIVE_BEGIN;
    UTempestMathLibrary::GenericSortWeaponArryByLvIdDesc(ArrayAddr, ArrayProp, FirstID, SecondID);
    P_NATIVE_END;
}

static FArrayProperty* FindColArrayPropertyCaseInsensitive(UScriptStruct* InStruct)
{
    if (!InStruct) return nullptr;

    auto MatchesCol = [](const FString& Name) -> bool
        {
            if (Name.IsEmpty()) return false;
            // 去掉蓝图/引擎生成的后缀（如 col_0_XXXX）
            int32 UnderIdx = Name.Find(TEXT("_"));
            const FString Head = UnderIdx != INDEX_NONE ? Name.Left(UnderIdx) : Name;
            return Head.Equals(TEXT("col"), ESearchCase::IgnoreCase);
        };

    for (TFieldIterator<FProperty> It(InStruct); It; ++It)
    {
        FProperty* Prop = *It;
        if (!Prop) continue;

#if ENGINE_MAJOR_VERSION >= 5
        const FString Authored = Prop->GetAuthoredName();
        if (MatchesCol(Authored))
        {
            return CastField<FArrayProperty>(Prop);
        }
#endif
        const FString Internal = Prop->GetName();
        if (MatchesCol(Internal))
        {
            return CastField<FArrayProperty>(Prop);
        }
    }
    return nullptr;
}

void UTempestMathLibrary::SortWeaponArryByLvIdDesc(TArray<int32>& /*WeaponArry*/, int32 /*firstID*/, int32 /*secondID*/)
{
    // 空实现：逻辑在 CustomThunk 中执行
}

void UTempestMathLibrary::GenericSortWeaponArryByLvIdDesc(void* TargetArray, const FArrayProperty* ArrayProp, int32 FirstID, int32 SecondID)
{
    if (!TargetArray || !ArrayProp || !ArrayProp->Inner)
    {
        return;
    }

    // 必须是结构体数组
    const FStructProperty* ElementStructProp = CastField<FStructProperty>(ArrayProp->Inner);
    if (!ElementStructProp || !ElementStructProp->Struct)
    {
        return;
    }

    UScriptStruct* ElementStruct = ElementStructProp->Struct;

    // 方案1：找到名为 col 的 int 数组（大小写不敏感）
    FArrayProperty* ColArrayProp = FindColArrayPropertyCaseInsensitive(ElementStruct);
    FIntProperty* ColInnerIntProp = ColArrayProp ? CastField<FIntProperty>(ColArrayProp->Inner) : nullptr;

    // 方案2（回退）：寻找独立的 Id / Lv (或 Level) int 字段（大小写不敏感）
    FIntProperty* IdProp = nullptr;
    FIntProperty* LvProp = nullptr;
    if (!ColArrayProp || !ColInnerIntProp)
    {
        for (TFieldIterator<FProperty> It(ElementStruct); It; ++It)
        {
            FProperty* Prop = *It;
            if (!Prop) continue;
            if (!IdProp && Prop->GetName().Equals(TEXT("id"), ESearchCase::IgnoreCase))
            {
                IdProp = CastField<FIntProperty>(Prop);
            }
            if (!LvProp && (Prop->GetName().Equals(TEXT("lv"), ESearchCase::IgnoreCase) ||
                Prop->GetName().Equals(TEXT("level"), ESearchCase::IgnoreCase)))
            {
                LvProp = CastField<FIntProperty>(Prop);
            }
        }
    }

    // 两种方式都不可用则返回
    if (!(ColArrayProp && ColInnerIntProp) && !(IdProp && LvProp))
    {
        return;
    }

    FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
    const int32 N = ArrayHelper.Num();
    if (N <= 1)
    {
        return;
    }

    // 提取指定元素的 ID/Lv
    auto GetIdLvAt = [&](int32 Index, int32& OutId, int32& OutLv)
        {
            OutId = 0;
            OutLv = 0;

            void* ElementPtr = ArrayHelper.GetRawPtr(Index);

            if (ColArrayProp && ColInnerIntProp)
            {
                void* ColContainerPtr = ColArrayProp->ContainerPtrToValuePtr<void>(ElementPtr);
                if (!ColContainerPtr) return;

                FScriptArrayHelper ColHelper(ColArrayProp, ColContainerPtr);
                const int32 ColCount = ColHelper.Num();

                if (ColCount >= 1)
                {
                    OutId = ColInnerIntProp->GetPropertyValue(ColHelper.GetRawPtr(0));   // [0] -> ID
                }
                if (ColCount >= 2)
                {
                    OutLv = ColInnerIntProp->GetPropertyValue(ColHelper.GetRawPtr(1));   // [1] -> Lv
                }
            }
            else if (IdProp && LvProp)
            {
                void* IdAddr = IdProp->ContainerPtrToValuePtr<void>(ElementPtr);
                void* LvAddr = LvProp->ContainerPtrToValuePtr<void>(ElementPtr);
                if (IdAddr) OutId = IdProp->GetPropertyValue(IdAddr);
                if (LvAddr) OutLv = LvProp->GetPropertyValue(LvAddr);
            }
        };

    auto FindIndexById = [&](int32 SearchId) -> int32
        {
            if (SearchId == 0) return INDEX_NONE;
            for (int32 i = 0; i < ArrayHelper.Num(); ++i)
            {
                int32 Id, Lv;
                GetIdLvAt(i, Id, Lv);
                if (Id == SearchId)
                {
                    return i;
                }
            }
            return INDEX_NONE;
        };

    // 先按 Lv 降序；Lv 相同按 ID 降序
    for (int32 i = 0; i < N - 1; ++i)
    {
        bool bSwapped = false;
        for (int32 j = 0; j < N - i - 1; ++j)
        {
            int32 IdA, LvA, IdB, LvB;
            GetIdLvAt(j, IdA, LvA);
            GetIdLvAt(j + 1, IdB, LvB);

            const bool bSwap = (LvA < LvB) || (LvA == LvB && IdA < IdB);
            if (bSwap)
            {
                ArrayHelper.SwapValues(j, j + 1);
                bSwapped = true;
            }
        }
        if (!bSwapped) break;
    }

    // 再执行“置顶/置二”规则
    // 规则1：firstID 放到索引 0（firstID==0 则跳过）
    if (FirstID != 0)
    {
        int32 FirstIdx = FindIndexById(FirstID);
        if (FirstIdx != INDEX_NONE && FirstIdx > 0)
        {
            for (int32 k = FirstIdx; k > 0; --k)
            {
                ArrayHelper.SwapValues(k, k - 1);
            }
        }
    }

    // 规则2：secondID 放到索引 1（secondID==0 则跳过；若与 firstID 相同则无额外操作）
    if (SecondID != 0 && SecondID != FirstID)
    {
        int32 SecondIdx = FindIndexById(SecondID);
        if (SecondIdx != INDEX_NONE)
        {
            if (SecondIdx == 0)
            {
                // 若当前在 0 位且不是 firstID，则与索引 1 交换，确保其在第二位
                if (ArrayHelper.Num() >= 2)
                {
                    ArrayHelper.SwapValues(0, 1);
                }
            }
            else if (SecondIdx > 1)
            {
                for (int32 k = SecondIdx; k > 1; --k)
                {
                    ArrayHelper.SwapValues(k, k - 1);
                }
            }
            // 若 SecondIdx == 1，已满足
        }
    }
}