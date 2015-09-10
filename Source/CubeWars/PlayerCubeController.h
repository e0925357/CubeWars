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

private:
	bool bCanShoot;
	
};
