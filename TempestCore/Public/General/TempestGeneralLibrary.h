// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Objects/TempestSpecialProperty.h"
#include "Objects/TempestBaseConditionObject.h"
#include "TempestGeneralLibrary.generated.h"

/**
 *
 */
UCLASS()
class TEMPESTCORE_API UTempestGeneralLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable, Category = "Tempest General Functions")
    static UObject* GetObjectOfClass(const TArray<UObject*>& InObjectsArray, TSubclassOf<UObject> InObjectClass);

    UFUNCTION(BlueprintCallable, Category = "Tempest General Functions")
    static UTempestSpecialProperty* GetSpecialPropertyOfTag(const TArray<FInstancedSpecialProperties>& InSpecialPropertiesArray, FGameplayTag InGameplayTag);

    UFUNCTION(BlueprintCallable, Category = "Duplication Base Functions", meta = (WorldContext = "WorldContextObject"))
    static FInstancedConditionProperties CreateDuplicateForCondition(AActor* ActorOwner, FInstancedConditionProperties ConditionToDuplicate);

    UFUNCTION(BlueprintCallable, Category = "Duplication Base Functions", meta = (WorldContext = "WorldContextObject"))
    static FInstancedSpecialProperties CreateDuplicateForSpecialProperty(AActor* ActorOwner, FInstancedSpecialProperties PropertyToDuplicate);

    UFUNCTION(BlueprintCallable, Category = "Tempest General Functions", meta = (WorldContext = "WorldContextObject"))
    static bool GetIsPawnControlledByPlayer(class APawn* InPawn);

    UFUNCTION(BlueprintCallable, Category = "Tempest General Functions", meta = (WorldContext = "WorldContextObject"))
    static TArray<class UInputAction*> GetInputsOfInputContext(class UInputMappingContext* InInputContext);
};
