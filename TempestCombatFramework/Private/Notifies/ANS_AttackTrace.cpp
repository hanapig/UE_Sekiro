// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 


#include "Notifies/ANS_AttackTrace.h"
#include "Misc/UObjectToken.h"


#if WITH_EDITOR
void UANS_AttackTrace::ValidateAssociatedAssets()
{
	static const FName NAME_AssetCheck("AssetCheck");

// 	if (AttackProperty == nullptr)
// 	{
// 		UObject* ContainingAsset = GetContainingAsset();
// 
// 		FMessageLog AssetCheckLog(NAME_AssetCheck);
// 
// 		const FText MessageLooping = FText::Format(
// 			NSLOCTEXT("AnimNotify", "AttackProperty", "Attack Property Is Not Set"),
// 			FText::AsCultureInvariant(AttackProperty->GetPathName()),
// 			FText::AsCultureInvariant(ContainingAsset->GetPathName()));
// 		AssetCheckLog.Warning()
// 			->AddToken(FUObjectToken::Create(ContainingAsset))
// 			->AddToken(FTextToken::Create(MessageLooping));
// 
// 		if (GIsEditor)
// 		{
// 			AssetCheckLog.Notify(MessageLooping, EMessageSeverity::Warning, /*bForce=*/ true);
// 		}
// 	}
}
#endif