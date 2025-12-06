// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "TempestBaseBuffObject.generated.h"

/**
 *
 */
UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseBuffObject : public UTempestBaseObject
{
    GENERATED_BODY()
public:

    UTempestBaseBuffObject();

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Base Variables")
    FGameplayTag BuffGameplayTag;

private:
    UPROPERTY()
    TArray<AActor*> DamageDealers;

    UPROPERTY()
    AActor* DamageReceiver;

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Base Functions")
    TArray<AActor*> GetDamageDealers() { return DamageDealers; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Base Functions")
    FORCEINLINE AActor* GetDamageReceiver() { return DamageReceiver; }

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetDamageDealer(AActor* NewDamageDealer);

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetDamageReceiver(AActor* NewDamageReceiver);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void StartBuff();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ReapplyBuff();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void EndBuff();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    bool GetCanApplyBuff();
};
