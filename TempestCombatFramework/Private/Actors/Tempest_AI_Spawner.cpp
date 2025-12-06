// Created By Aamn Chahrour & Samrudh Sunil. All Rights Reserved.


#include "Actors/Tempest_AI_Spawner.h"
#include "Engine/Selection.h"
#include "DrawDebugHelpers.h"


ATempest_AI_Spawner::ATempest_AI_Spawner()
{
#if WITH_EDITOR
	if (GIsEditor)
	{
		USelection::SelectObjectEvent.AddUObject(this, &ATempest_AI_Spawner::OnObjectSelected);
	}
#endif
}
#if WITH_EDITOR
void ATempest_AI_Spawner::OnObjectSelected(UObject* Object)
{
	TArray<AActor*> ActorsEffected;
	if (Object == this)
	{
		bSelectedInEditor = true;
		if (SpawnProcessor.SpawnerProcessor)
		{
			ActorsEffected = SpawnProcessor.SpawnerProcessor->GetActorsToHide();
			for (AActor*& Elem : ActorsEffected)
			{
				if (Elem)
				{
					Elem->SetIsTemporarilyHiddenInEditor(false);
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("The Selected Spawner is %s"), *Object->GetName());
	}
	else
	{
		if (bSelectedInEditor) 
		{
			if (SpawnProcessor.SpawnerProcessor)
			{
				AActor* SelectedActor = Cast<AActor>(Object);
				ActorsEffected = SpawnProcessor.SpawnerProcessor->GetActorsToHide();
				if (!(ActorsEffected.Contains(SelectedActor)))
				{
					for (AActor*& Elem : ActorsEffected)
					{
						if (Elem)
						{
							Elem->SetIsTemporarilyHiddenInEditor(true);
						}
					}
					bSelectedInEditor = false;
				}

			}
		}

		
	}
}
#endif

void ATempest_AI_Spawner::AssignProcessor()
{
	if (SpawnProcessor.SpawnerProcessor)
	{
		UTempestSpawnerProcessor* LocalNewSpawnerProcessor = DuplicateObject(SpawnProcessor.SpawnerProcessor, this);
		if (LocalNewSpawnerProcessor)
		{
			SpawnProcessor.SpawnerProcessor = LocalNewSpawnerProcessor;
			SpawnProcessor.SpawnerProcessor->ConstructProcessor();
		}
	}
	
}
