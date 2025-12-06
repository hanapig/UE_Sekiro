// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TempestBasePlayerController.h"
#include "Debug/TempestDebugObject.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/HUD.h"

ATempestBasePlayerController::ATempestBasePlayerController()
{

}

void ATempestBasePlayerController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn->IsLocallyControlled())
	{
		if (TempestDebugObject)
		{
			TempestDebugObject->UpdateDebugging(InPawn);
		}
		else
		{
			TempestDebugObject = NewObject<UTempestDebugObject>(this, UTempestDebugObject::StaticClass());
			if (TempestDebugObject)
			{
				TempestDebugObject->UpdateDebugging(InPawn);
				GetHUD()->OnShowDebugInfo.AddUObject(TempestDebugObject, &UTempestDebugObject::OnShowDebugInfo);
			}
		}

	}
}
