// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "CubeDeathController.h"
#include "PlayerCube.h"
#include "CubeDebris.h"
#include "PlayerCubeController.h"
#include "CubeWarsPlayerState.h"
#include "CubeWarsGameMode.h"


ACubeDeathController::ACubeDeathController()
	: targetHeight(300)
	, breakTime(1)
	, breakTimer(0)
	, waiting(false)
	, explosionForce(5000)
	, teamNumber(0)
{
}

void ACubeDeathController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	breakTimer = breakTime;
}

void ACubeDeathController::actorReachedHeight()
{
	waiting = true;
}

void ACubeDeathController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if(waiting)
	{
		if(breakTimer >= 0)
		{
			breakTimer -= DeltaTime;
		}
		else
		{

			// Spawn the shockwave
			playerCube->OnDeath(explosionForce);

			//Tell the gamemode that we are dead
			if(GetWorld() != nullptr)
			{
				ACubeWarsGameMode* cwgm = GetWorld()->GetAuthGameMode<ACubeWarsGameMode>();

				cwgm->playerDied(teamNumber);
			}

			//Destroy actor
			GetPawn()->Destroy();
		}
	}
}

void ACubeDeathController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	playerCube = Cast<APlayerCube>(InPawn);

	if(playerCube != nullptr)
	{
		//tell cube to rise
		playerCube->startRaising(targetHeight);
	}
}

void ACubeDeathController::setTeamNumer(int32 teamNumber)
{
	this->teamNumber = teamNumber;
}
