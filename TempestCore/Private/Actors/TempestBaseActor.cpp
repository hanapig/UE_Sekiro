// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved. 


#include "Actors/TempestBaseActor.h"

// Sets default values
ATempestBaseActor::ATempestBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void ATempestBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATempestBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

