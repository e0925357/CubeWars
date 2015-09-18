// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "CubeWarsGameMode.h"
#include "PlayerCube.h"
#include "PlayerCubeController.h"
#include "CubeWarsPlayerState.h"
#include "CubeWarsGameState.h"
#include "ObstacleMovementComponent.h"
#include "OrbitingPawn.h"

namespace
{
	// TODO: Remove these hardcoded teddybears and replace the with a spawn area in the editor
	const float MinYPos = -1000.0f;
	const float MaxYPos = 1000.0f;
	const float MinXPos = -500.0f;
	const float MaxXPos = 500.0f;
}

namespace MatchState
{
	const FName Fight = TEXT("Fight");
	const FName AfterDeathDelay = TEXT("AfterDeathDelay");
}

ACubeWarsGameMode::ACubeWarsGameMode()
	: DefaultDestroyableObstacle(ADestroyableObstacle::StaticClass())
	, NumObstacles(3)
	, ObstacleRespawnTime(5.0f)
	, WaitTime(4)
	, startTimer(3.0f)
	, nextSecond(0)
	, winnerTeam(-1)
	, firstPlayerRematch(false)
	, secondPlayerRematch(false)
	, PickupRespawnTime(15.0f)
	, PickupClass(APickup::StaticClass())
	, pickupRespawnTimer(30.0f)
	, nextGUID(0)
	, AfterDeathTime(1.5f)
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

	//Determine the team this player will be on
	bool foundTeam1 = false;
	bool foundTeam2 = false;

	for(FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
	{
		APlayerController* playerController = *iter;
		ACubeWarsPlayerState* cwPlayerState = Cast<ACubeWarsPlayerState>(playerController->PlayerState);

		if(cwPlayerState != nullptr)
		{
			switch(cwPlayerState->GetTeamNumber())
			{
			case 1:
				foundTeam1 = true;
				break;
			case 2:
				foundTeam2 = true;
				break;
			}
		}
	}

	if(!foundTeam1)
	{
		//Make him a member of team 1
		ACubeWarsPlayerState* cwPlayerState = Cast<ACubeWarsPlayerState>(NewPlayer->PlayerState);

		if(cwPlayerState != nullptr)
		{
			cwPlayerState->SetTeamNumber(1);
		}
	}
	else if(!foundTeam2)
	{
		//Make him a member of team 1
		ACubeWarsPlayerState* cwPlayerState = Cast<ACubeWarsPlayerState>(NewPlayer->PlayerState);

		if(cwPlayerState != nullptr)
		{
			cwPlayerState->SetTeamNumber(2);
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
	//Reset rematch-requests
	firstPlayerRematch = false;
	secondPlayerRematch = false;
	afterDeathTimer = AfterDeathTime;

	ObstacleArray.Empty();
	ObstacleRespawnArray.Empty();

	ObstacleArray.AddZeroed(NumObstacles);
	ObstacleRespawnArray.AddZeroed(NumObstacles);

	//Create obstacles
	for (int32 i = 0; i < NumObstacles; ++i)
	{
		SpawnObstacle(i);
	}

	pickupRespawnTimer = PickupRespawnTime;
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
	return GetMatchState() == MatchState::AfterDeathDelay && afterDeathTimer <= 0;
}

void ACubeWarsGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetMatchState() == MatchState::Fight)
	{
		// Tick code to call when fighting

		// Update respawn of obstacles
		for (int32 i = 0; i < ObstacleRespawnArray.Num(); ++i)
		{
			if(ObstacleArray[i] != nullptr)
			{
				continue;
			}

			ObstacleRespawnArray[i] += DeltaSeconds;

			if(ObstacleRespawnArray[i] >= ObstacleRespawnTime)
			{
				SpawnObstacle(i);
			}
		}

		//Update pickup-spawning
		pickupRespawnTimer -= DeltaSeconds;

		if(pickupRespawnTimer < 0.0f)
		{
			pickupRespawnTimer = PickupRespawnTime;
			SpawnPickup();
		}

		//Next state?
		if(winnerTeam > 0)
		{
			SetMatchState(MatchState::AfterDeathDelay);
		}
	}
	else if(GetMatchState() == MatchState::InProgress)
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
	else if(GetMatchState() == MatchState::AfterDeathDelay)
	{
		afterDeathTimer -= DeltaSeconds;
	}

	//Should we end the match?
	if(IsMatchInProgress() && ReadyToEndMatch())
	{
		EndMatch();
	}
}

bool ACubeWarsGameMode::IsMatchInProgress() const
{
	if(GetMatchState() == MatchState::InProgress || GetMatchState() == MatchState::Fight || GetMatchState() == MatchState::AfterDeathDelay)
	{
		return true;
	}

	return false;
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
	winnerTeam = -1;

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

void ACubeWarsGameMode::playerDied(int32 team)
{
	if(team == 1)
	{
		winnerTeam = 2;
	}
	else if(team == 2)
	{
		winnerTeam = 1;
	}

	//Add a point to the winning team
	for(FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
	{
		APlayerController* playerController = *iter;
		ACubeWarsPlayerState* cwPlayerState = Cast<ACubeWarsPlayerState>(playerController->PlayerState);

		if(cwPlayerState != nullptr && cwPlayerState->GetTeamNumber() == winnerTeam)
		{
			cwPlayerState->AddPoints();
		}
	}
}

void ACubeWarsGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	UWorld* world = GetWorld();

	if(world == nullptr)
	{
		return;
	}

	FString player1Name = TEXT("Player 1");
	FString player2Name = TEXT("Player 2");
	int32 player1Points = 0;
	int32 player2Points = 0;
	const FVector OrbitCenter(0.0f, 0.0f, 250.0f);
	const FVector OrbitCamOffset(650.0f, 0.0f, 250.0f);

	//Gather data
	for(FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
	{
		APlayerController* playerController = *iter;

		APlayerCubeController* playerCubeController = Cast<APlayerCubeController>(playerController);

		if(playerCubeController != nullptr)
		{
			ACubeWarsPlayerState* playerState = Cast<ACubeWarsPlayerState>(playerCubeController->PlayerState);

			if(playerState != nullptr)
			{
				int32 playerTeam = playerState->GetTeamNumber();

				//unpossess old pawn, create & possess orbiting spectators
				if(playerCubeController->GetPawn()->IsValidLowLevel())
					ActorsToRemove.Add(playerCubeController->GetPawn());

				playerCubeController->UnPossess();

				AOrbitingPawn* orbitingPawn = GetWorld()->SpawnActor<AOrbitingPawn>(AOrbitingPawn::StaticClass());
				orbitingPawn->SetupOrbitMulticast(OrbitCenter, 1500.0f, 3.0f);

				playerCubeController->Possess(orbitingPawn);

				//Get points
				switch(playerTeam)
				{
				case 1:
					player1Points = playerState->GetPoints();
					break;

				case 2:
					player2Points = playerState->GetPoints();
					break;
				}

				//Get player Name
				if(!playerCubeController->getPlayerName().IsEmpty())
				{
					switch(playerTeam)
					{
					case 1:
						player1Name = playerCubeController->getPlayerName();
						break;

					case 2:
						player2Name = playerCubeController->getPlayerName();
						break;
					}
				}
			}
		}
	}

	//Show off the winner
	for(FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
	{
		APlayerController* playerController = *iter;

		APlayerCubeController* playerCubeController = Cast<APlayerCubeController>(playerController);

		bool hasWon = false;

		if(playerCubeController != nullptr)
		{
			ACubeWarsPlayerState* playerState = Cast<ACubeWarsPlayerState>(playerCubeController->PlayerState);

			if(playerState != nullptr)
			{
				hasWon = winnerTeam == playerState->GetTeamNumber();
			}

			playerCubeController->matchEnded(player1Name, player1Points, player2Name, player2Points, hasWon);
		}
	}
}

void ACubeWarsGameMode::ObstacleDied(int32 ObstacleIndex)
{
	// First check whether the obstacle is already respawning
	if(ObstacleRespawnArray[ObstacleIndex] > 0.0f)
	{
		return;
	}

	ObstacleArray[ObstacleIndex] = nullptr;
}

void ACubeWarsGameMode::SpawnObstacle(int32 ObstacleIndex)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = this;
	SpawnParameters.bDeferConstruction = false;

	FRandomStream RandStream;
	RandStream.GenerateNewSeed();

	float XPos = MinXPos + (MaxXPos - MinXPos) * (static_cast<float>(ObstacleIndex) / (NumObstacles - 1));
	float YPos = RandStream.FRandRange(MinYPos, MaxYPos);

	ADestroyableObstacle* obstacle = GetWorld()->SpawnActor<ADestroyableObstacle>(DefaultDestroyableObstacle, FVector(XPos, YPos, 90.0f), FRotator::ZeroRotator, SpawnParameters);
	obstacle->SetObstacleIndex(ObstacleIndex);
	obstacle->GetObstacleMovementComponent()->SetMovingRightMulticast(RandStream.RandRange(0, 1) != 0);

	ObstacleArray[ObstacleIndex] = obstacle;
	ObstacleRespawnArray[ObstacleIndex] = 0.0f;
}

void ACubeWarsGameMode::requestRematch(int32 team)
{
	if(team == 1)
	{
		firstPlayerRematch = true;
	}
	else if(team == 2)
	{
		secondPlayerRematch = true;
	}

	if((firstPlayerRematch || secondPlayerRematch) && (firstPlayerRematch != secondPlayerRematch))
	{
		//Show off the winner
		for(FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
		{
			APlayerController* playerController = *iter;

			APlayerCubeController* playerCubeController = Cast<APlayerCubeController>(playerController);

			if(playerCubeController != nullptr)
			{
				playerCubeController->ClientRequestRestart();
			}
		}
	}

	if(firstPlayerRematch && secondPlayerRematch)
	{
		//*** Restart Level
		//Remove all obstacles
		for(int32 i = 0; i < NumObstacles; ++i)
		{
			if(ObstacleArray[i] != nullptr && !ObstacleArray[i]->IsActorBeingDestroyed())
				ObstacleArray[i]->Destroy();

			ObstacleArray[i] = nullptr;
			ObstacleRespawnArray[i] = 0.0f;
		}

		//Remove marked actors
		for(int32 i = 0; i < ActorsToRemove.Num(); i++)
		{
			ActorsToRemove[i]->Destroy();
		}

		ActorsToRemove.Empty();

		//Remove all pawns & notify them that the game restarts now
		for(FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
		{
			APlayerController* playerController = *iter;

			APawn* controlledPawn = playerController->GetPawn();

			if(controlledPawn->IsValidLowLevel())
			{
				controlledPawn->Destroy();
			}

			APlayerCubeController* playerCubeController = Cast<APlayerCubeController>(playerController);

			if(playerCubeController != nullptr)
			{
				playerCubeController->MatchRestarted();
			}
		}

		SetMatchState(MatchState::WaitingToStart);
	}
}

void ACubeWarsGameMode::SpawnPickup()
{
	if(PowerUpClasses.Num() == 0 || PowerUpSpawnWeights.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Pickups specified - can't spawn Pickups!"));
		return;
	}

	if(PowerUpClasses.Num() != PowerUpSpawnWeights.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerUpClasses and PowerUpSpawnWeights differ in size - can't spawn Pickups!"));
		return;
	}

	int32 weightSum = 0;

	for(int32 i = 0; i < PowerUpSpawnWeights.Num(); i++)
	{
		weightSum += PowerUpSpawnWeights[i];
	}

	FRandomStream RandStream;
	RandStream.GenerateNewSeed();

	int32 randomNumber = RandStream.RandRange(0, weightSum);
	TSubclassOf<APowerUp> chosenPowerup;
	weightSum = 0;

	for(int32 i = 0; i < PowerUpSpawnWeights.Num(); i++)
	{
		weightSum += PowerUpSpawnWeights[i];

		if(randomNumber <= weightSum)
		{
			chosenPowerup = PowerUpClasses[i];
			break;
		}
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = this;
	float XPos = RandStream.FRandRange(-100, 100);
	float YPos = RandStream.FRandRange(MinYPos, MaxYPos);

	APickup* Pickup = GetWorld()->SpawnActor<APickup>(PickupClass, FVector(XPos, YPos, 90.0f), FRotator::ZeroRotator, SpawnParameters);
	APowerUp* PowerUp = GetWorld()->SpawnActor<APowerUp>(chosenPowerup);

	Pickup->SetGUID(nextGUID++);
	PowerUp->SetGUID(nextGUID++);

	Pickup->SetPowerUp(PowerUp->GetGUID());
	PowerUp->setPickupId(Pickup->GetGUID());
}