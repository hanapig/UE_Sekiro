// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "TempestDataContainer.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class EInputBufferType : uint8
{
    LastRegisteredInput		UMETA(DisplayName = "Last Registered Input"),
    InstantInputFire		UMETA(DisplayName = "Instant Input Fire"),
    HighestPriorityInput	UMETA(DisplayName = "Highest Priority Input"),
};

UENUM(BlueprintType)
enum class EActivationStatus : uint8
{
    None					UMETA(DisplayName = "None"),
    Activated				UMETA(DisplayName = "Activated"),
    Deactivated				UMETA(DisplayName = "Deactivated"),
};

USTRUCT(BlueprintType, meta = (DisplayName = "Montages & Tags"))
struct FMontagesAndGameplayTags
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Variables")
    FGameplayTag MontagesTag;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Variables")
    TArray <class UAnimMontage*> AnimMontages;

    FMontagesAndGameplayTags() : MontagesTag(FGameplayTag::RequestGameplayTag(FName("List.One"))), AnimMontages() {};
    ~FMontagesAndGameplayTags() {};
};

USTRUCT(BlueprintType, meta = (DisplayName = "Montages & Tags Array"))
struct FMontagesAndGameplayTagsArray
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Variables")
    TArray < FMontagesAndGameplayTags> MontagesAndGameplayTagsArray;

    FMontagesAndGameplayTagsArray() :MontagesAndGameplayTagsArray() {};
    ~FMontagesAndGameplayTagsArray() {};
};

USTRUCT(BlueprintType, meta = (DisplayName = "Array Of Vectors"))
struct FArrayOfVectors
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Variables")
    TArray <FVector> VectorLocation;

    FArrayOfVectors() : VectorLocation() {};
    ~FArrayOfVectors() {};
};

UCLASS()
class TEMPESTCORE_API UTempestDataContainer : public UTempestBaseObject
{
    GENERATED_BODY()
};
