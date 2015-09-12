// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerCubeController.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API APlayerCubeController : public APlayerController
{
	GENERATED_BODY()
public:

	void setCanShoot(bool bCanShoot);

	UFUNCTION(Client, Reliable)
	void setCanShootClient(bool bCanShoot);

	bool canShoot();

	UFUNCTION(Client, Reliable)
	void showFullscreenMessage(const FString& text);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShowFullscreenMessage(const FString& text);

	UFUNCTION(Client, Reliable)
	void matchEnded(const FString& player1Name, int32 player1Points, const FString& player2Name, int32 player2Points, bool hasWon);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMatchEnded(const FString& player1Name, int32 player1Points, const FString& player2Name, int32 player2Points, bool hasWon);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void BPRequestRestart();

	UFUNCTION(Server, WithValidation, reliable)
	void ServerRequestRestart();

	UFUNCTION(Client, reliable)
	void MatchRestarted();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMatchRestart();

	UFUNCTION(BlueprintCallable, Category = Stats)
	void setPlayerNameBP(const FString& name);

	UFUNCTION(Server, WithValidation, reliable)
	void setPlayerNameServer(const FString& name);

	const FString& getPlayerName();

private:
	bool bCanShoot;
	
	UPROPERTY()
	FString playerName;
};
