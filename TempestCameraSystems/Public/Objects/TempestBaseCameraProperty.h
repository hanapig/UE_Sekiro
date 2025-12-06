// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "TempestBaseCameraProperty.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Instanced Camera Properties"))
struct FInstancedCameraProperty
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, Category = "Instanced Variables")
    class UTempestBaseCameraProperty* CameraPropertyToCreate;

    FInstancedCameraProperty() : CameraPropertyToCreate(nullptr) {};
    ~FInstancedCameraProperty() {};
};
UCLASS(Abstract, BlueprintType, EditInlineNew)
class TEMPESTCAMERASYSTEMS_API UTempestBaseCameraProperty : public UTempestBaseObject
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
    FGameplayTag PropertyType;

    UPROPERTY(BlueprintReadWrite, Category = "Initialization")
    FGameplayTag CameraPropertyStatus;

    UPROPERTY(BlueprintReadOnly, Category = "Initialization")
    class UTempestBaseCameraMode* CameraMode;

protected:

    UPROPERTY(BlueprintReadWrite, Category = "Tick Values")
    float TimePassed;

public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ConstructCameraProperty();
    virtual void ConstructCameraProperty_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void StartCameraProperty();
    virtual void StartCameraProperty_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void ProcessCameraProperty();
    virtual void ProcessCameraProperty_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
    void UpdateCameraPropertyStatus(FGameplayTag NewStatus);
    virtual void UpdateCameraPropertyStatus_Implementation(FGameplayTag NewStatus);

    UFUNCTION(BlueprintCallable, Category = "Base Functions")
    void SetCameraMode(UTempestBaseCameraMode* NewCameraMode) { CameraMode = NewCameraMode; }
};
