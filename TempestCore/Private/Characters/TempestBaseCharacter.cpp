// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved.

#include "Characters/TempestBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATempestBaseCharacter::ATempestBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void ATempestBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATempestBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsLocallyControlled())
    {
        FHitResult OutHit;
        GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, 0.001f), GetActorRotation(), true, OutHit);
        GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, -0.001f), GetActorRotation(), true, OutHit);
    }
}

// Called to bind functionality to input
void ATempestBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}