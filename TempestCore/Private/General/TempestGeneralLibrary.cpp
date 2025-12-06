// Fill out your copyright notice in the Description page of Project Settings.

#include "General/TempestGeneralLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"

UObject* UTempestGeneralLibrary::GetObjectOfClass(const TArray<UObject*>& InObjectsArray, TSubclassOf<UObject> InObjectClass)
{
    if (InObjectsArray.IsEmpty())
    {
        return nullptr;
    }

    for (int32 i = 0; i < InObjectsArray.Num(); i++)
    {
        if (InObjectsArray[i])
        {
            if (InObjectsArray[i] && InObjectsArray[i]->IsA(InObjectClass))
            {
                return InObjectsArray[i]; // Return the object if it matches the class
            }
        }
    }

    return nullptr;
}

UTempestSpecialProperty* UTempestGeneralLibrary::GetSpecialPropertyOfTag(const TArray<FInstancedSpecialProperties>& InSpecialPropertiesArray, FGameplayTag InGameplayTag)
{
    if (InSpecialPropertiesArray.IsEmpty())
    {
        return nullptr;
    }

    for (const FInstancedSpecialProperties& Property : InSpecialPropertiesArray)
    {
        if (Property.SpecialProperty && Property.SpecialProperty->PropertyTag == InGameplayTag)
        {
            return Property.SpecialProperty;
        }
    }

    return nullptr;
}

FInstancedConditionProperties UTempestGeneralLibrary::CreateDuplicateForCondition(AActor* ActorOwner, FInstancedConditionProperties ConditionToDuplicate)
{
    FInstancedConditionProperties LocalCondition;
    if (ConditionToDuplicate.Condition)
    {
        if (ActorOwner)
        {
            UTempestBaseConditionObject* LocalNewCondition = DuplicateObject(ConditionToDuplicate.Condition, ActorOwner);
            if (LocalNewCondition)
            {
                LocalCondition.Condition = LocalNewCondition;
            }
        }
    }
    return LocalCondition;
}

FInstancedSpecialProperties UTempestGeneralLibrary::CreateDuplicateForSpecialProperty(AActor* ActorOwner, FInstancedSpecialProperties PropertyToDuplicate)
{
    FInstancedSpecialProperties LocalProperty;
    if (PropertyToDuplicate.SpecialProperty && ActorOwner)
    {
        UTempestSpecialProperty* LocalNewProperty = DuplicateObject(PropertyToDuplicate.SpecialProperty, ActorOwner);
        if (LocalNewProperty)
        {
            LocalProperty.SpecialProperty = LocalNewProperty;
        }
    }
    return LocalProperty;
}

bool UTempestGeneralLibrary::GetIsPawnControlledByPlayer(class APawn* InPawn)
{
    if (InPawn != nullptr)
    {
        if (InPawn->GetController() != nullptr)
        {
            APlayerController* LocalPlayerController = Cast<APlayerController>(InPawn->GetController());
            if (LocalPlayerController != nullptr)
            {
                return true;
            }
        }
    }

    return false;
}

TArray<class UInputAction*> UTempestGeneralLibrary::GetInputsOfInputContext(class UInputMappingContext* InInputContext)
{
    TArray<UInputAction*> AllInputActions;

    if (InInputContext != nullptr)
    {
        TArray<FEnhancedActionKeyMapping> LocalInputMappings = InInputContext->GetMappings();
        for (FEnhancedActionKeyMapping& Elem : LocalInputMappings)
        {
            if (Elem.Action != nullptr)
            {
                AllInputActions.Emplace(Elem.Action);
            }
        }
    }

    return AllInputActions;
}