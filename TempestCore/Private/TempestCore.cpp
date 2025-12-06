#include "TempestCore.h"

DEFINE_LOG_CATEGORY(TempestCore);

#define LOCTEXT_NAMESPACE "FTempestCore"

void FTempestCore::StartupModule()
{
	UE_LOG(TempestCore, Warning, TEXT("TempestCore module has been loaded"));
}

void FTempestCore::ShutdownModule()
{
	UE_LOG(TempestCore, Warning, TEXT("TempestCore module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTempestCore, TempestCore)