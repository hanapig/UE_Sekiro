// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Data/TempestDataContainer.h"
#include "Engine/HitResult.h"
#include "TempestBaseTraceObject.generated.h"

/**
 *
 */
class UTempestAttackPropertiesObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitDelegate, FHitResult, HitResult, FGameplayTag, TraceGameplayTag);

UCLASS(Abstract, AutoExpandCategories = ("Initialization | Trace Info", "Initialization | Mesh Info", "Initialization | Collision Trace"))
class TEMPESTCOMBATFRAMEWORK_API UTempestBaseTraceObject : public UTempestBaseObject
{
    GENERATED_BODY()
public:

    /* Trace Information */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
    FGameplayTag TraceGameplayTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
    FGameplayTagContainer AcceptableActorsTags;

    UPROPERTY(BlueprintReadWrite, Category = "Trace Info")
    TMap<FName, FArrayOfVectors> TraceTrajectory;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Dispatchers")
    FOnHitDelegate OnTraceHitDelegate;

    /* Trace Mesh Information */
    UPROPERTY(BlueprintReadWrite, Category = "Initialization | Mesh Info")
    class UPrimitiveComponent* TracePrimitiveComponent;

    UPROPERTY(BlueprintReadWrite, Category = "Initialization | Mesh Info")
    TArray<FName> TracePrimitiveComponentSocketNames;

    UPROPERTY(BlueprintReadOnly, Category = "Trace Properties")
    AActor* TraceOwner = nullptr;

    UPROPERTY(BlueprintReadWrite, Category = "Trace Properties")
    TArray<AActor*> HitActors;

    UPROPERTY(BlueprintReadWrite, Category = "Trace Properties")
    TArray<UPrimitiveComponent*> HitComponents;

    UPROPERTY(BlueprintReadOnly, Category = "Trace Properties | Attack")
    UTempestAttackPropertiesObject* AttackProperty;

    UPROPERTY(BlueprintReadWrite, Category = "Trace Properties | Attack")
    FHitResult CurrentHitResult;

public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Trace Base Functions")
    void ConstructTrace();
    virtual void ConstructTrace_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Trace Base Functions")
    void ToggleTrace(bool bToggle);
    virtual void ToggleTrace_Implementation(bool bToggle);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Trace Base Functions")
    void OnTraceHit(FHitResult HitResult);
    virtual void OnTraceHit_Implementation(FHitResult HitResult);

    /* Getters */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Trace Base Functions | Getters")
    bool GetCanHitTargetActor(const AActor* TargetActor);
    virtual bool GetCanHitTargetActor_Implementation(const AActor* TargetActor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Trace Base Functions | Getters")
    bool GetCanHitTargetComponent(const UPrimitiveComponent* TargetComponent);
    virtual bool GetCanHitTargetComponent_Implementation(const UPrimitiveComponent* TargetComponent);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Trace Base Functions | Getters")
    void GetCollisionTraceHits(TArray<FHitResult>& FoundHitResult);
    void GetCollisionTraceHits_Implementation(TArray<FHitResult>& FoundHitResult);

    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Trace Base Functions | Getters")
    class AController* GetTraceInstigator();

    /* Setters */
    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetTraceMeshInfo(UPrimitiveComponent* NewPrimitiveComponent, TArray<FName> PrimitiveComponentSocketNames);

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetTraceOwner(AActor* InNewTraceOwner);

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void AddTraceCurrentTrajectory(const FName& InBoneName, const FVector& InCurrentTraceTrajectoryLocation);

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void ConstructAttackPropertyOfObject(UTempestAttackPropertiesObject* AttackPropertyToAssign);

    /* This function is the function that fires the sphere trace */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void DoCollisionTrace();
    virtual void DoCollisionTrace_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void RecordCollisionTrajectory();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ClearTraceInformation();
};

UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UBaseTraceByChannelObject : public UTempestBaseTraceObject
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere Trace Info")
    TEnumAsByte<ETraceTypeQuery> DefaultTraceType;
};

UCLASS()
class TEMPESTCOMBATFRAMEWORK_API UBaseSphereTraceByChannelObject : public UBaseTraceByChannelObject
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere Trace Info")
    float DefaultTraceRadius = 1.f;
};