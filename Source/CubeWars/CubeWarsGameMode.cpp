// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "CubeWarsGameMode.h"
#include "PlayerCube.h"
#include "PlayerCubeController.h"
#include "CubeWarsPlayerState.h"
#include "CubeWarsGameState.h"
#include "ObstacleMovementComponent.h"

namespace MatchState
{
	const FName Fight = TEXT("Fight");
}

ACubeWarsGameMode::ACubeWarsGameMode()
	: DefaultDestroyableObstacle(ADestroyableObstacle::StaticClass())
	, NumObstacles(3), startTimer(3.0f), nextSecond(0), WaitTime(4)
{
	GameStateClass = ACubeWarsGameState::StaticClass();
	DefaultPawnClass = APlayerCube::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
	PlayerStateClass = ACubeWarsPlayerState::StaticClass();
}

void ACubeWarsGameMode::PreLogin(const FString& Options, const FString& Address, const TSharedPtr<const FUniqueNetId>& UniqueId, FString& ErrorMessage)
{
	if (GameState->PlayerArray.Num() >= 2)
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
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
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

		ADestroyableObstacle* obstacle = GetWorld()->SpawnActor<ADestroyableObstacle>(DefaultDestroyableObstacle, FVector(XPos, YPos, 90.0f), FRotator::ZeroRotator, SpawnParameters);
		obstacle->GetObstacleMovementComponent()->MovingRight = RandStream.RandRange(0, 1) != 0;
	}
}

/** @return True if ready to Start Match. Games should override this */
bool ACubeWarsGameMode::ReadyToStartMatch_Implementation()
{
	if(GetMatchState() != MatchState::WaitingToStart || NumPlayers < 2)
	{
		return false;
	}

	UWorld* world = GetWorld();

	if(world == nullptr)
	{
		return false;
	}

	//Check if all players have already loaded the world in
	for(FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
	{
		APlayerController* playerController = *iter;

		if(!playerController->HasClientLoadedCurrentWorld())
		{
			return false;
		}
	}

	return true;
}

/** @return true if ready to End Match. Games should override this */
bool ACubeWarsGameMode::ReadyToEndMatch_Implementation()
{
	//TODO: implement end condition
	return false;
}

void ACubeWarsGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetMatchState() == MatchState::InProgress)
	{
		startTimer -= DeltaSeconds;

		UWorld* world = GetWorld();

		if(world == nullptr)
		{
			return;
		}

		if(startTimer <= nextSecond)
		{
			if(nextSecond > 0)
			{
				//Show next number
				for(FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
				{
					APlayerController* playerController = *iter;

					APlayerCubeController* playerCubeController = Cast<APlayerCubeController>(playerController);

					if(playerCubeController != nullptr)
					{
						playerCubeController->showFullscreenMessage(FString::FromInt(nextSecond));
					}
				}
			}
			else
			{
				//Show GO message
				for(FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
				{
					APlayerController* playerController = *iter;

					APlayerCubeController* playerCubeController = Cast<APlayerCubeController>(playerController);

					if(playerCubeController != nullptr)
					{
						playerCubeController->showFullscreenMessage(TEXT("GO!"));
					}
				}
			}

			--nextSecond;
		}

		if(startTimer <= 0)
		{

			//Enable shots for all players
			for(FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
			{
				APlayerController* playerController = *iter;

				APlayerCubeController* playerCubeController = Cast<APlayerCubeController>(playerController);

				if(playerCubeController != nullptr)
				{
					playerCubeController->setCanShoot(true);
				}
			}

			SetMatchState(MatchState::Fight);
		}
	}
}

void ACubeWarsGameMode::StartMatch()
{
	if(HasMatchStarted())
	{
		// Already started
		return;
	}

	//Let the game session override the StartMatch function, in case it wants to wait for arbitration
	if(GameSession->HandleStartMatchRequest())
	{
		return;
	}

	startTimer = WaitTime;
	nextSecond = WaitTime;

	UWorld* world = GetWorld();

	if(world == nullptr)
	{
		return;
	}

	//Disable shots for all players
	for(FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
	{
		APlayerController* playerController = *iter;

		APlayerCubeController* playerCubeController = Cast<APlayerCubeController>(playerController);

		if(playerCubeController != nullptr)
		{
			playerCubeController->setCanShoot(false);
		}
	}

	SetMatchState(MatchState::InProgress);
}
