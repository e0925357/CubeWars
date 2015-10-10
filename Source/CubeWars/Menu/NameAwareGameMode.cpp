// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "NameAwareGameMode.h"


/** Adds the player name to the name list */
void ANameAwareGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//Is the new player a named player?
	ANamedPlayerController* PlayerCubeController = Cast<ANamedPlayerController>(NewPlayer);

	if(PlayerCubeController != nullptr)
	{
		UninitializedControllersArray.Add(PlayerCubeController);
	}
}

/** Removes the player name from the name list */
void ANameAwareGameMode::Logout(AController* Exiting)
{
	//Is the exiting player a cube wars player?
	ANamedPlayerController* PlayerCubeController = Cast<ANamedPlayerController>(Exiting);

	if(PlayerCubeController != nullptr)
	{
		if(UninitializedControllersArray.Contains(PlayerCubeController))
		{
			UninitializedControllersArray.Remove(PlayerCubeController);
		}
		else
		{
			NameArray.Remove(PlayerCubeController->getPlayerName());
		}
	}

	PlayerNamesChange();
	OnPlayerNamesChange();

	Super::Logout(Exiting);
}

void ANameAwareGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	bool bNameUpdated = false;

	//Check for uninitialized player cube controllers
	for(int32 i = 0; i < UninitializedControllersArray.Num(); ++i)
	{
		ANamedPlayerController* PlayerCubeController = UninitializedControllersArray[i];

		if(PlayerCubeController->IsNameInitialized())
		{
			NameArray.Add(PlayerCubeController->getPlayerName());
			UninitializedControllersArray.RemoveAt(i);
			bNameUpdated = true;
			--i;
		}
	}

	if(bNameUpdated)
	{
		PlayerNamesChange();
		OnPlayerNamesChange();
	}
}

const TArray<FString>& ANameAwareGameMode::GetPlayerNames() const
{
	return NameArray;
}

void ANameAwareGameMode::PlayerNamesChange()
{
	FString Name1 = NameArray.Num() > 0 ? NameArray[0] : TEXT("");
	FString Name2 = NameArray.Num() > 1 ? NameArray[1] : TEXT("");

	//Remove all pawns & notify them that the game restarts now
	for(FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
	{
		APlayerController* playerController = *iter;

		//Is the player a named player?
		ANamedPlayerController* PlayerCubeController = Cast<ANamedPlayerController>(playerController);

		if(PlayerCubeController != nullptr)
		{
			PlayerCubeController->UpdatePlayerNamesClient(Name1, Name2);
		}
	}
}

