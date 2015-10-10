// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "NamedPlayerController.h"


ANamedPlayerController::ANamedPlayerController() : bNameInitialized(false), playerName()
{

}

void ANamedPlayerController::setPlayerNameBP(const FString& name)
{
	playerName = name;
	bNameInitialized = true;
	setPlayerNameServer(name);
}

void ANamedPlayerController::setPlayerNameServer_Implementation(const FString& name)
{
	playerName = name;
	bNameInitialized = true;
}

bool ANamedPlayerController::setPlayerNameServer_Validate(const FString& name)
{
	return true;
}

const FString& ANamedPlayerController::getPlayerName()
{
	return playerName;
}

bool ANamedPlayerController::IsNameInitialized() const
{
	return bNameInitialized;
}

void ANamedPlayerController::UpdatePlayerNamesClient_Implementation(const FString& Player1Name, const FString& Player2Name)
{
	OnUpdatePlayerNames(Player1Name, Player2Name);
}

