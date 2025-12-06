// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/TempestBaseTargetingObject.h"

UTempestBaseTargetingObject::UTempestBaseTargetingObject()
{

}

void UTempestBaseTargetingObject::ConstructTargetingObject_Implementation()
{

}

void UTempestBaseTargetingObject::ToggleTargeting_Implementation(bool ToggleValue)
{

}

void UTempestBaseTargetingObject::UpdateTargeting_Implementation()
{

}

void UTempestBaseTargetingObject::SetTargetedComponent(UTempestTargetSceneComponent* NewTargetedComponent)
{
	TargetedComponent = NewTargetedComponent;
	OnUpdatedTargetedComponent.Broadcast();
}
