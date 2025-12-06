// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "TempestBaseAttributeObject.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FAttributeProperties
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attribute Properties")
    float AttributeValue;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attribute Properties")
    float MinAttributeValue;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attribute Properties")
    float MaxAttributeValue;

    FAttributeProperties() :AttributeValue(0.0f), MinAttributeValue(0.0f), MaxAttributeValue(0.0f) {};
    ~FAttributeProperties() {};
};

USTRUCT(BlueprintType, meta = (DisplayName = "Instanced Attributes"))
struct FInstancedAttributes
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestBaseAttributeObject* AttributeToCreate;

    FInstancedAttributes() :AttributeToCreate(nullptr) {};
    ~FInstancedAttributes() {};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueUpdated, FGameplayTag, UpdatedValue);

UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseAttributeObject : public UTempestBaseObject
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Dispatchers")
    FOnValueUpdated OnValueUpdated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute Properties", meta = (ExposeOnSpawn = true))
    FAttributeProperties AttributeValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute Properties", meta = (ExposeOnSpawn = true))
    FGameplayTag AttributeGameplayTag;

protected:
    UPROPERTY()
    AActor* OwningActor;

private:

public:
    /* Attribute Main Functions */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attribute Base Functions")
    void ConstructAttribute();
    virtual void ConstructAttribute_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attribute Base Functions")
    void ModifyAttribute(float ModifyBy);
    virtual void ModifyAttribute_Implementation(float ModifyBy);

    UFUNCTION(BlueprintCallable, Category = "Attribute Setters")
    void SetOwningActor(AActor* NewOwningActor) { OwningActor = NewOwningActor; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attribute Getters")
    void GetOwningActor(AActor*& CurrentOwningActor) { CurrentOwningActor = OwningActor; }
};
