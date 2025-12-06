// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#include "TempestCameraSystems.h"

DEFINE_LOG_CATEGORY(TempestCameraSystems);

#define LOCTEXT_NAMESPACE "FTempestCameraSystems"

void FTempestCameraSystems::StartupModule()
{
	UE_LOG(TempestCameraSystems, Warning, TEXT("TempestCameraSystems module has been loaded"));
}

void FTempestCameraSystems::ShutdownModule()
{
	UE_LOG(TempestCameraSystems, Warning, TEXT("TempestCameraSystems module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTempestCameraSystems, TempestCameraSystems)