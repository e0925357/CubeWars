// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "CubeWarsGameMode.h"
#include "PlayerCube.h"
#include "PlayerCubeController.h"
#include "CubeWarsPlayerState.h"
#include "CubeWarsGameState.h"

ACubeWarsGameMode::ACubeWarsGameMode()
	: NumObstacles(3)
{
	GameStateClass = ACubeWarsGameState::StaticClass();
	DefaultPawnClass = APlayerCube::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
	PlayerStateClass = ACubeWarsPlayerState::StaticClass();
}

void ACubeWarsGameMode::PreLogin(const FString& Options, const FString& Address, const TSharedPtr<class FUniqueNetId>& UniqueId, FString& ErrorMessage)
{
	if (GameState->PlayerArray.Num() == 2)
	{
		// Thou shall not haz more than two players
		ErrorMessage = TEXT("Match is full!");
	}
	else
	{
		Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	}
}

void ACubeWarsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ACubeWarsPlayerState* NewPlayerState = CastChecked<ACubeWarsPlayerState>(NewPlayer->PlayerState);

	switch (GameState->PlayerArray.Num())
	{
		case 0:
		{
			NewPlayerState->SetTeamNumber(1);
			break;
		}
		case 1:
		{
			NewPlayerState->SetTeamNumber(2);
			break;
		}
		default:
		{
			// TODO: Thou shall not happen and is prevented in PreLogin
		}
	}
}

AActor* ACubeWarsGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	APlayerCubeController* CubeController = Cast<APlayerCubeController>(Player);
	ACubeWarsPlayerState* PlayerState = Cast<ACubeWarsPlayerState>(Player->PlayerState);

	
	int32 PlayerNumber = 0;
	if (!HasMatchStarted())
	{
		// Match has not started and players are spawning
		PlayerNumber = GameState->PlayerArray.Num();
	}
	else
	{
		// Match is in progress and player is respawning
		PlayerNumber = PlayerState->GetTeamNumber();
	}

	// Determine the tag of the spawn point for the player
	const char* SpawnPlayerStartTag = "";
	if (GameState->PlayerArray.Num() == 1)
	{
		// Player 1
		SpawnPlayerStartTag = "1";
	}
	else if (GameState->PlayerArray.Num() == 2)
	{
		// Player 2
		SpawnPlayerStartTag = "2";
	}

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* TestSpawn = *It;

		// A "Play in Editor" spawn point doesn't make sense in this game at all
		if (!TestSpawn->bIsPIEPlayerStart)
		{
			if (TestSpawn->PlayerStartTag == SpawnPlayerStartTag)
			{
				// This is player one and we have just found his player start. Yay!
				return TestSpawn;
			}
			else if (TestSpawn->PlayerStartTag == SpawnPlayerStartTag)
			{
				// This is player two and we have just found his player start. Yay!
				return TestSpawn;
			}
		}
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ACubeWarsGameMode::HandleMatchIsWaitingToStart()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoCollisionFail = false;
	SpawnParameters.Owner = this;
	SpawnParameters.bDeferConstruction = false;

	// TODO: Remove these hardcoded teddybears and replace the with a spawn area in the editor
	float MinYPos = -1000.0f;
	float MaxYPos = 1000.0f;
	float MinXPos = -500.0f;
	float MaxXPos = 500.0f;

	FRandomStream RandStream;
	RandStream.GenerateNewSeed();

	for (int32 i = 0; i < NumObstacles; ++i)
	{

		float XPos = MinXPos + (MaxXPos - MinXPos) * (static_cast<float>(i) / (NumObstacles - 1));
		float YPos = RandStream.FRandRange(MinYPos, MaxYPos);

		AObstacle* obstacle = GetWorld()->SpawnActor<AObstacle>(DefaultObstacle, FVector(XPos, YPos, 90.0f), FRotator::ZeroRotator, SpawnParameters);
		obstacle->MovingRight = RandStream.RandRange(0, 1) != 0;
	}
}