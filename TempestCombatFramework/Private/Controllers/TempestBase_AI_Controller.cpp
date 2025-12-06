// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TempestBase_AI_Controller.h"
#include "Navigation/CrowdFollowingComponent.h"

ATempestBase_AI_Controller::ATempestBase_AI_Controller(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ATempestBase_AI_Controller::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	GetSightPerceptionViewPoint(out_Location, out_Rotation);
}

void ATempestBase_AI_Controller::GetSightPerceptionViewPoint_Implementation(FVector& out_Location, FRotator& out_Rotation) const
{

}
