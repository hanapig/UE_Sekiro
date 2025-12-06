// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#include "TempestCombatFramework.h"
#include "Interfaces/IPluginManager.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FTempestCombatFrameworkModule"

void FTempestCombatFrameworkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	TSharedPtr<IPlugin> ThisPlugin = IPluginManager::Get().FindPlugin(TEXT("TempestCombatFramework"));
	if (ThisPlugin.IsValid())
	{
		UGameplayTagsManager::Get().AddTagIniSearchPath(ThisPlugin->GetBaseDir() / TEXT("Config") / TEXT("Tags"));
	}
	
}

void FTempestCombatFrameworkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTempestCombatFrameworkModule, TempestCombatFramework)