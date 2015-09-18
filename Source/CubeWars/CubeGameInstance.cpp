// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "CubeGameInstance.h"


UCubeGameInstance::UCubeGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UCubeGameInstance::SetPlayerName(const FString& PlayerName)
{
	this->PlayerName = PlayerName;
}

const FString& UCubeGameInstance::GetPlayerName()
{
	return PlayerName;
}

