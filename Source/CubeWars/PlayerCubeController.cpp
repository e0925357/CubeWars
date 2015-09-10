// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "PlayerCubeController.h"


void APlayerCubeController::setCanShoot(bool bCanShoot)
{
	this->bCanShoot = bCanShoot;

	setCanShootClient(bCanShoot);
}

void APlayerCubeController::setCanShootClient_Implementation(bool bCanShoot)
{
	this->bCanShoot = bCanShoot;
}

void APlayerCubeController::showFullscreenMessage_Implementation(const FString& text)
{
	OnShowFullscreenMessage(text);
}

bool APlayerCubeController::canShoot()
{
	return bCanShoot;
}
