// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"

#include "Obstacle.h"
#include "DestroyableObstacle.h"

#include "CubeWarsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API ACubeWarsGameMode : public AGameMode
{
	GENERATED_BODY()

	ACubeWarsGameMode();
	
	/** Before the login. Prevents that more than two players connect */
	virtual void PreLogin(const FString& Options, const FString& Address, const TSharedPtr<const FUniqueNetId>& UniqueId, FString& ErrorMessage) override;

	/** Starts the match warmup and determines the team of the player */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Chooses the start of the player based on his team */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	/** Match is waiting to start and players are now spawned in */
	virtual void HandleMatchIsWaitingToStart() override;

	/** @return True if ready to Start Match. Games should override this */
	virtual bool ReadyToStartMatch_Implementation() override;

	/** @return true if ready to End Match. Games should override this */
	virtual bool ReadyToEndMatch_Implementation() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayControl)
	TSubclassOf<AObstacle> DefaultObstacle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayControl)
	TSubclassOf<ADestroyableObstacle> DefaultDestroyableObstacle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayControl)
	int32 NumObstacles;

private:
	float startTimer;
	bool bStartTimer;
};
