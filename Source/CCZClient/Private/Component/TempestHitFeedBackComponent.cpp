// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TempestHitFeedBackComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTempestHitFeedBackComponent::UTempestHitFeedBackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTempestHitFeedBackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTempestHitFeedBackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTempestHitFeedBackComponent::FrameRate(float DelayTime)
{
    if (DelayTime <= 0.f) return;

    AActor* Owner = GetOwner();
    if (!Owner) return;

	Owner ->CustomTimeDilation = 0.0001f;
	
    // 计时恢复
    FTimerDelegate RestoreDelegate = FTimerDelegate::CreateLambda(
        [Owner]()
        {
        	Owner ->CustomTimeDilation = 1.f;
        });

    if (GetWorld())
        GetWorld()->GetTimerManager().SetTimer(HitReactionTimerHandle, RestoreDelegate, DelayTime, false);

}
