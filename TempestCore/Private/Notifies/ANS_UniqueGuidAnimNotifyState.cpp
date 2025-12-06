// Copyright 2024, Developed by Aamn Chahrour, Published by Inherited Tempest. All Rights Reserved. 


#include "Notifies/ANS_UniqueGuidAnimNotifyState.h"

void UANS_UniqueGuidAnimNotifyState::GenerateNewGuid()
{
	AnimNotifyStateGuid = FGuid::NewGuid();
}
