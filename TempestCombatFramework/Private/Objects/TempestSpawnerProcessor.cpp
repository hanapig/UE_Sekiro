// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/TempestSpawnerProcessor.h"
#include "Engine/StreamableManager.h"
#include "Actors/Tempest_AI_Spawner.h"
#include "Engine/AssetManager.h"

UTempestSpawnerProcessor::UTempestSpawnerProcessor()
{

}

void UTempestSpawnerProcessor::ProcessProcessor_Implementation()
{

}


ATempest_AI_Spawner* UTempestSpawnerProcessor::GetSpawner() const
{
    return SpawnerRef;
}

void UTempestSpawnerProcessor::ConstructProcessor()
{
    SpawnerRef = Cast<ATempest_AI_Spawner>(GetOuter());

    if (!(ProcessorConditions.DestructionCondition && ProcessorConditions.SpawnCondition))
    {
        return;
    }

    UTempestBaseConditionObject* LocalNewSpawnCondition = DuplicateObject(ProcessorConditions.SpawnCondition, this);
    if (LocalNewSpawnCondition)
    {
        ProcessorConditions.SpawnCondition = LocalNewSpawnCondition;
    }

    UTempestBaseConditionObject* LocalNewDestructionCondition = DuplicateObject(ProcessorConditions.DestructionCondition, this);
    if (LocalNewDestructionCondition)
    {
        ProcessorConditions.DestructionCondition = LocalNewDestructionCondition;
    }

    if (ProcessorConditions.DestructionCondition && ProcessorConditions.SpawnCondition)
    {
        ProcessProcessor();
    }
}

TArray<AActor*> UTempestSpawnerProcessor::GetActorsToHide_Implementation()
{   
    TArray<AActor*> LocalEmptyArray;
    return LocalEmptyArray;
}

void UTempestSpawnerProcessor::StartLoadingAssets(TArray<TSoftClassPtr<AActor>> AssetsToLoad)
{
    // Create an instance of StreamableManager
    FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

    // Create an array to hold the asset references
    TArray<FSoftObjectPath> AssetReferences;


    // Populate the AssetReferences array with the object paths of the assets to load
    for (TSoftClassPtr<AActor>& Asset : AssetsToLoad)
    {
        AssetReferences.Emplace(Asset.ToSoftObjectPath());
    }

    // Create a delegate for the callback
    FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &UTempestSpawnerProcessor::OnAssetsLoadComeplete);

    // Request asynchronous loading of the assets
    StreamableManager.RequestAsyncLoad(AssetReferences, OnAssetsLoadedDelegate);

}

void UTempestSpawnerProcessor::OnAssetsLoadComeplete_Implementation()
{

}
