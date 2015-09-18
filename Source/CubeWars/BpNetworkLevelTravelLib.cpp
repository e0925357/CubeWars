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

	/** Helper function to perform the client travel code */
	bool ClientTravel(const FString& URLString, UObject* WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
		check(World);
		UEngine* const Engine = World->GetGameInstance()->GetEngine();
		check(Engine);
		FWorldContext* WorldContext = Engine->GetWorldContextFromWorld(World);
		check(WorldContext);

		FURL TestURL(&WorldContext->LastURL, *URLString, TRAVEL_Absolute);
		if (TestURL.IsLocalInternal())
		{
			// make sure the file exists if we are opening a local file
			if (!Engine->MakeSureMapNameIsValid(TestURL.Map))
			{
				FConsoleOutputDevice StrOut(GetConsole());
				StrOut.Logf(TEXT("ERROR: The map '%s' does not exist."), *TestURL.Map);
				return false;
			}
		}

		Engine->SetClientTravel(World, *URLString, TRAVEL_Absolute);

		return true;
	}
}

bool UBpNetworkLevelTravelLib::OpenLocalServer(const FString& LevelName, UObject* WorldContextObject)
{
	FString URLString = LevelName + FString("?Listen");

	return ClientTravel(URLString, WorldContextObject);
}

bool UBpNetworkLevelTravelLib::ConnectToServer(const FString& IpAddress, UObject* WorldContextObject)
{
	return ClientTravel(IpAddress, WorldContextObject);
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