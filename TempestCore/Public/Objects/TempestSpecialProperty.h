// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "GameplayTagContainer.h"
#include "TempestSpecialProperty.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Instanced Special Properties"))
struct FInstancedSpecialProperties
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestSpecialProperty* SpecialProperty;

    FInstancedSpecialProperties()
        : SpecialProperty(nullptr)
    {
    };

    ~FInstancedSpecialProperties()
    {
    }
};

UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCORE_API UTempestSpecialProperty : public UTempestBaseObject
{
    GENERATED_BODY()

public:
    UTempestSpecialProperty();

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Special Property Base Variables")
    FGameplayTag PropertyTag;

private:
    UObject* PropertyOwner;

public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Special Property Base Functions")
    void OnPropertyCreated(UObject* NewPropertyOwner);
    virtual void OnPropertyCreated_Implementation(UObject* NewPropertyOwner);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Special Property Base Functions")
    void GetPropertyOwner(UObject*& CurrentPropertyOwner) { CurrentPropertyOwner = PropertyOwner; }

    UFUNCTION(BlueprintCallable, Category = "Special Property Base Functions")
    void SetPropertyOwner(UObject* NewOwner);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Special Property Base Functions")
    bool GetCanProcessProperty();
    virtual bool GetCanProcessProperty_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Special Property Base Functions")
    void ProcessSpecialProperty();
    virtual void ProcessSpecialProperty_Implementation();
};
