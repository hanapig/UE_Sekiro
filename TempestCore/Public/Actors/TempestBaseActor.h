// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "TempestBaseActor.generated.h"

UCLASS()
class TEMPESTCORE_API ATempestBaseActor : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATempestBaseActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tempest Initialization")
	FGameplayTagContainer ActorGameplayTags;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = ActorGameplayTags; return; }
};
