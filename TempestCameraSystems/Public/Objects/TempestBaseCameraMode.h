// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Objects/TempestBaseObject.h"
#include "TempestBaseCameraProperty.h"
#include <Templates/SubclassOf.h>
#include "TempestBaseCameraMode.generated.h"

/**
 * 
 */
 class UTempestBaseCameraProperty;
UCLASS()
class TEMPESTCAMERASYSTEMS_API UTempestBaseCameraMode : public UTempestBaseObject
{
	GENERATED_BODY()
public:
	UTempestBaseCameraMode();

	UPROPERTY(BlueprintReadWrite, Category = "Initialization")
	FGameplayTag CameraModeStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	TArray<FInstancedCameraProperty> CameraProperties;

	UPROPERTY(BlueprintReadOnly, Category = "Base Variables")
	class UTempestCameraModeComponent* CameraModeComponent;

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Tick Values")
	float TimePassed;



public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
	void ConstructCameraMode();
	virtual void ConstructCameraMode_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
	void InitializeCameraProperties();
	virtual void InitializeCameraProperties_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
	void StartCameraMode();
	virtual void StartCameraMode_Implementation();
	
	UFUNCTION(BlueprintCallable, Category = "Camera Mode Setters")
	void SetCameraModeStatus(FGameplayTag NewCameraModeStatus);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Camera Mode Setters")
	void OnUpdatedCameraModeStatus();
	virtual void OnUpdatedCameraModeStatus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Functions")
	void EndCameraMode();
	virtual void EndCameraMode_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintPure ,Category = "Camera Mode Getters")
	void GetCameraPropertyOfType(FGameplayTag PropertyTypeToFind, UTempestBaseCameraProperty*& FoundCameraProperty);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera Mode Getters")
	void GetCameraPropertyOfClass(TSubclassOf<UTempestBaseCameraProperty> PropertyClassToFind, UTempestBaseCameraProperty*& FoundCameraProperty);



	
};
