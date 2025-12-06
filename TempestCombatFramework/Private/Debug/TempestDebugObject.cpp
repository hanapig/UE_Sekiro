// Fill out your copyright notice in the Description page of Project Settings.

#include "Debug/TempestDebugObject.h"
#include "Components/TempestBaseAbilityManagerComponent.h"
#include "Components/TempestBaseStateManagerComponent.h"
#include "Components/TempestCombatComponent.h"
#include "Components/TempestTickingComponent.h"
#include "DisplayDebugHelpers.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

UTempestDebugObject::UTempestDebugObject()
{
}

void UTempestDebugObject::UpdateDebugging(class APawn* ControlledPawn)
{
    if (GetOuter() && ControlledPawn)
    {
        SaveComponentsReferences(ControlledPawn);
    }
}

void UTempestDebugObject::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
    if (DisplayInfo.IsDisplayOn(TEXT("TempestCombat")))
    {
        int32 CurrentCount = 1;
        // header
        Canvas->SetDrawColor(FColor::White);
        Canvas->DrawText(GEngine->GetSmallFont(), TEXT("Tempest Combat Debug Info"), 100.f, YPos);
        //YPos += YL;

        /*----------------------------------*/

        if (AbilityComponent)
        {
            YPos += 50;// Adjust YPos for the next piece of information.
            FString AbilityString = TEXT("Current Active Ability: ");;
            FString CurrentActiveAbilityString = TEXT("None");;
            if (AbilityComponent->GetCurrentActiveAbility())
            {
                AbilityComponent->GetCurrentActiveAbility()->GetName(CurrentActiveAbilityString);
            }
            AbilityString.Append(CurrentActiveAbilityString);
            Canvas->SetDrawColor(FColor(153, 255, 255));
            Canvas->DrawText(GEngine->GetSmallFont(), AbilityString, 4.f, YPos);
        }

        if (StateComponent)
        {
            YPos += 20;
            FString StateString = TEXT("Current Active State: ");
            FString CurrentActiveStateString = TEXT("None");
            if (StateComponent->GetCurrentActiveState())
            {
                StateComponent->GetCurrentActiveState()->GetName(CurrentActiveStateString);
            }

            StateString.Append(CurrentActiveStateString);
            Canvas->SetDrawColor(FColor(153, 255, 153));
            Canvas->DrawText(GEngine->GetSmallFont(), StateString, 4.f, YPos);
        }

        /*----------------------------------*/
        if (AbilityComponent)
        {
            YPos += 50;
            FString PassiveAbilitiesString;
            PassiveAbilitiesString.Append("All Passive Abilities:   ");
            TArray<FString> PassiveAbilitiesValues;
            CurrentCount = 1;

            TArray<UTempestBaseAbilityObject*> LocalPassiveAbilities;
            AbilityComponent->GetPassiveAbilities(LocalPassiveAbilities);

            for (UTempestBaseAbilityObject*& Elem : LocalPassiveAbilities)
            {
                if (Elem)
                {
                    PassiveAbilitiesValues.Emplace(Elem->GetName());
                }
            }

            for (FString& Elem : PassiveAbilitiesValues)
            {
                PassiveAbilitiesString.Append(FString::Printf(TEXT("%s "), *Elem));
                if (CurrentCount++ < PassiveAbilitiesValues.Num())
                {
                    PassiveAbilitiesString += TEXT("/ ");
                }
            }
            Canvas->SetDrawColor(FColor(153, 255, 255));
            Canvas->DrawText(GEngine->GetSmallFont(), PassiveAbilitiesString, 4.f, YPos);
        }

        /*----------------------------------*/

        if (StateComponent)
        {
            YPos += 20;
            FString PassiveStatesString;
            PassiveStatesString.Append("All Passive States:   ");
            TArray<FString> PassiveStatessValues;
            CurrentCount = 1;
            TArray<UTempestBaseStateObject*> LocalPassiveStates;
            StateComponent->GetPassiveStates(LocalPassiveStates);

            for (UTempestBaseStateObject*& Elem : LocalPassiveStates)
            {
                if (Elem)
                {
                    PassiveStatessValues.Emplace(Elem->GetName());
                }
            }

            for (FString& Elem : PassiveStatessValues)
            {
                PassiveStatesString.Append(FString::Printf(TEXT("%s "), *Elem));
                if (CurrentCount++ < PassiveStatessValues.Num())
                {
                    PassiveStatesString += TEXT("/ ");
                }
            }
            Canvas->SetDrawColor(FColor(153, 255, 153));
            Canvas->DrawText(GEngine->GetSmallFont(), PassiveStatesString, 4.f, YPos);
        }

        /*----------------------------------*/

        if (CombatComponent)
        {
            YPos += 50;
            FString CombatStatusString;
            CombatStatusString.Append("Combat Status:   ");
            TArray<FString> CombatStatusValues;
            CurrentCount = 1;

            TArray<UTempestBaseAbilityObject*> LocalPassiveAbilities;
            AbilityComponent->GetPassiveAbilities(LocalPassiveAbilities);

            for (FGameplayTag Elem : CombatComponent->GetCombatStatus())
            {
                CombatStatusValues.Emplace(Elem.ToString());
            }

            for (FString& Elem : CombatStatusValues)
            {
                CombatStatusString.Append(FString::Printf(TEXT("%s "), *Elem));
                if (CurrentCount++ < CombatStatusValues.Num())
                {
                    CombatStatusString += TEXT("/ ");
                }
            }
            Canvas->SetDrawColor(FColor(153, 204, 255));
            Canvas->DrawText(GEngine->GetSmallFont(), CombatStatusString, 4.f, YPos);
        }

        /*----------------------------------*/

        if (TickComponent)
        {
            YPos += 50;
            FString TickingObjectsString;
            TickingObjectsString.Append("Ticking Objects:   ");
            TArray<FString> TickingObjectsValue;
            CurrentCount = 1;

            TArray <UObject*> LocalFoundTickableObjects;
            TickComponent->GetTickableObjects(LocalFoundTickableObjects);

            for (UObject*& Elem : LocalFoundTickableObjects)
            {
                TickingObjectsValue.Emplace(Elem->GetName());
            }

            for (FString& Elem : TickingObjectsValue)
            {
                TickingObjectsString.Append(FString::Printf(TEXT("%s "), *Elem));
                if (CurrentCount++ < TickingObjectsValue.Num())
                {
                    TickingObjectsString += TEXT("/ ");
                }
            }
            Canvas->SetDrawColor(FColor(255, 255, 255));
            Canvas->DrawText(GEngine->GetSmallFont(), TickingObjectsString, 4.f, YPos, 1.0f, 1.0f);
        }

        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (!PlayerController)
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerController is null"));
            return;
        }

        FVector PlayerLocation;
        FRotator PlayerRotation;
        float SphereRadius = 1000.f;
        PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

        FVector ViewDirection = PlayerRotation.Vector();
        FVector SphereCenter = PlayerLocation + (ViewDirection * SphereRadius);
        // Initialize the object types array
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

        TArray<AActor*> OverlappingActors;
        UKismetSystemLibrary::SphereOverlapActors(
            GetWorld(),
            SphereCenter,
            SphereRadius,
            ObjectTypes,  // Only consider pawns which includes ACharacter
            ACharacter::StaticClass(),  // Filter by ACharacter class
            {},  // Actors to ignore
            OverlappingActors  // Output array of overlapping actors
        );

        // Log or process the overlapping actors
        for (AActor* OverlappingActor : OverlappingActors)
        {
            if (OverlappingActor != PlayerController->GetPawn())
            {
                FVector ActorLocation = OverlappingActor->GetActorLocation();
                FString NameToPrint = TEXT("None");
                if (UTempestBaseStateManagerComponent* const OverlappingActorStateComponent = OverlappingActor->FindComponentByClass<UTempestBaseStateManagerComponent>())
                {
                    if (OverlappingActorStateComponent->GetCurrentActiveState())
                    {
                        NameToPrint = OverlappingActorStateComponent->GetCurrentActiveState()->StateGameplayTag.ToString();
                    }
                    DrawDebugString(GetWorld(), ActorLocation + FVector(0, 0, 20), NameToPrint, nullptr, FColor::White, GetWorld()->GetDeltaSeconds());
                }
                if (UTempestBaseAbilityManagerComponent* const OverlappingActorAbilityComponent = OverlappingActor->FindComponentByClass<UTempestBaseAbilityManagerComponent>())
                {
                    if (OverlappingActorAbilityComponent->GetCurrentActiveAbility())
                    {
                        NameToPrint = OverlappingActorAbilityComponent->GetCurrentActiveAbility()->AbilityGameplayTag.ToString();
                    }
                    DrawDebugString(GetWorld(), ActorLocation + FVector(0, 0, 40), NameToPrint, nullptr, FColor::White, GetWorld()->GetDeltaSeconds());
                }
            }
        }
    }
}

void UTempestDebugObject::SaveComponentsReferences(class APawn* ControlledPawn)
{
    if (ControlledPawn)
    {
        AbilityComponent = ControlledPawn->FindComponentByClass<UTempestBaseAbilityManagerComponent>();
        CombatComponent = ControlledPawn->FindComponentByClass<UTempestCombatComponent>();
        StateComponent = ControlledPawn->FindComponentByClass<UTempestBaseStateManagerComponent>();
        TickComponent = ControlledPawn->FindComponentByClass<UTempestTickingComponent>();
    }
}