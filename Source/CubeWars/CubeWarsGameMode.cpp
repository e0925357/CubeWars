// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "CubeWarsGameMode.h"
#include "PlayerCube.h"
#include "PlayerCubeController.h"


ACubeWarsGameMode::ACubeWarsGameMode()
{
	DefaultPawnClass = APlayerCube::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
}

