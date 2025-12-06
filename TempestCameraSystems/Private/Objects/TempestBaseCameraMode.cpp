// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 


#include "Objects/TempestBaseCameraMode.h"
#include "Components/TempestCameraModeComponent.h"
#include "Objects/TempestBaseCameraProperty.h"

UTempestBaseCameraMode::UTempestBaseCameraMode()
{

}

void UTempestBaseCameraMode::ConstructCameraMode_Implementation()
{

}

void UTempestBaseCameraMode::InitializeCameraProperties_Implementation()
{

}

void UTempestBaseCameraMode::StartCameraMode_Implementation()
{

}

void UTempestBaseCameraMode::SetCameraModeStatus(FGameplayTag NewCameraModeStatus)
{
	CameraModeStatus = NewCameraModeStatus; 
	OnUpdatedCameraModeStatus();
}

void UTempestBaseCameraMode::OnUpdatedCameraModeStatus_Implementation()
{

}

void UTempestBaseCameraMode::EndCameraMode_Implementation()
{

}

void UTempestBaseCameraMode::GetCameraPropertyOfType(FGameplayTag PropertyTypeToFind, UTempestBaseCameraProperty*& FoundCameraProperty)
{
	UTempestBaseCameraProperty* LocalFoundCameraProperty = nullptr;
	FoundCameraProperty = LocalFoundCameraProperty;
	if (CameraProperties.Num() > 0)
	{
		for (int32 i = 0; i < CameraProperties.Num(); i++)
		{

			if (CameraProperties[i].CameraPropertyToCreate)
			{
				if (CameraProperties[i].CameraPropertyToCreate->PropertyType == PropertyTypeToFind)
				{
					
					FoundCameraProperty = CameraProperties[i].CameraPropertyToCreate;
					return;
				}
			}

		}
	}
}

void UTempestBaseCameraMode::GetCameraPropertyOfClass(TSubclassOf<UTempestBaseCameraProperty> PropertyClassToFind, UTempestBaseCameraProperty*& FoundCameraProperty)
{
	FoundCameraProperty = nullptr;
	if (CameraProperties.Num() > 0)
	{
		for (int32 i = 0; i < CameraProperties.Num(); i++)
		{

			if (CameraProperties[i].CameraPropertyToCreate)
			{
				if (CameraProperties[i].CameraPropertyToCreate->GetClass() == PropertyClassToFind)
				{

					FoundCameraProperty = CameraProperties[i].CameraPropertyToCreate;
					return;
				}
			}

		}
	}

}

