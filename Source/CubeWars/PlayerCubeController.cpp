// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "PlayerCubeController.h"
#include "CubeWarsPlayerState.h"
#include "CubeWarsGameMode.h"


void APlayerCubeController::setCanShoot(bool bCanShoot)
{
	this->bCanShoot = bCanShoot;

	setCanShootClient(bCanShoot);
}

void APlayerCubeController::setCanShootClient_Implementation(bool bCanShoot)
{
	this->bCanShoot = bCanShoot;
}

void APlayerCubeController::showFullscreenMessage_Implementation(const FString& text)
{
	OnShowFullscreenMessage(text);
}

void APlayerCubeController::showNotificationMessage_Implementation(const FString& text)
{
	OnShowNotificationMessage(text);
}

bool APlayerCubeController::canShoot()
{
	return bCanShoot;
}

void APlayerCubeController::matchEnded_Implementation(const FString& player1Name, int32 player1Points, const FString& player2Name, int32 player2Points, bool hasWon)
{
	OnMatchEnded(player1Name, player1Points, player2Name, player2Points, hasWon);
}

void APlayerCubeController::BPRequestRestart()
{
	ServerRequestRestart();
}

void APlayerCubeController::ServerRequestRestart_Implementation()
{
	int32 team = 0;

	ACubeWarsPlayerState* state = Cast<ACubeWarsPlayerState>(PlayerState);

	if(state != nullptr)
	{
		team = state->GetTeamNumber();
	}

	ACubeWarsGameMode* gameMode = GetWorld()->GetAuthGameMode<ACubeWarsGameMode>();
	
	if(gameMode != nullptr)
	{
		gameMode->requestRematch(team);
	}
}

bool APlayerCubeController::ServerRequestRestart_Validate()
{
	return true;
}

void APlayerCubeController::MatchRestarted_Implementation()
{
	OnMatchRestart();
}

void APlayerCubeController::setPlayerNameBP(const FString& name)
{
	playerName = name;

	setPlayerNameServer(name);
}

void APlayerCubeController::setPlayerNameServer_Implementation(const FString& name)
{
	playerName = name;
}

bool APlayerCubeController::setPlayerNameServer_Validate(const FString& name)
{
	return true;
}

const FString& APlayerCubeController::getPlayerName()
{
	return playerName;
}

void APlayerCubeController::ClientRequestRestart_Implementation()
{
	OnRequestRestart();
}

