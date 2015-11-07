// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "BpNetworkLevelTravelLib.h"

namespace
{
	/** Helper function to get the console. */
	UConsole* GetConsole()
	{
		UConsole* ViewportConsole = (GEngine->GameViewport != nullptr) ? GEngine->GameViewport->ViewportConsole : nullptr;
		return ViewportConsole;
	}
}

bool UBpNetworkLevelTravelLib::OpenLocalServer(const FString& LevelName, UObject* WorldContextObject)
{
	UGameplayStatics::OpenLevel(WorldContextObject, FName(*LevelName), true, "Listen");
	return true;
}

bool UBpNetworkLevelTravelLib::ConnectToServer(const FString& IpAddress, UObject* WorldContextObject)
{
	UGameplayStatics::OpenLevel(WorldContextObject, FName(*IpAddress), true);
	return true;
}

bool UBpNetworkLevelTravelLib::OpenLevelOnAllClients(FString LevelName, UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	check(World);
	UEngine* const Engine = World->GetGameInstance()->GetEngine();
	check(Engine);

	if (World->IsServer())
	{
		if (Engine->MakeSureMapNameIsValid(LevelName))
		{
			World->ServerTravel(LevelName);
			return true;
		}
		else
		{
			FConsoleOutputDevice StrOut(GetConsole());
			StrOut.Logf(TEXT("ERROR: The map '%s' is either short package name or does not exist."), *LevelName);
		}
	}

	return false;
}