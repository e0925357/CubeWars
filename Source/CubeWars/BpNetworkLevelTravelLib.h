// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BpNetworkLevelTravelLib.generated.h"

/**
 * Helper Blueprint library to start local servers, connect to a server, or change the level for all connected clients.
 */
UCLASS()
class CUBEWARS_API UBpNetworkLevelTravelLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Opens the given level and starts this client as a server client. Returns false on failure. */
	UFUNCTION(BlueprintCallable, Category = "Network Level Travel", meta = (WorldContext = "WorldContextObject"))
	static bool OpenLocalServer(const FString& LevelName, UObject* WorldContextObject);
	
	/** Connects to the server with the given address and loads the current level from the server. Returns false on failure. */
	UFUNCTION(BlueprintCallable, Category = "Network Level Travel", meta = (WorldContext = "WorldContextObject"))
	static bool ConnectToServer(const FString& IpAddress, UObject* WorldContextObject);

	/** Opens the given level on the server and on all the connects clients. Returns false on failure. */
	UFUNCTION(BlueprintCallable, Category = "Network Level Travel", meta = (WorldContext = "WorldContextObject"))
	static bool OpenLevelOnAllClients(FString LevelName, UObject* WorldContextObject);
};
