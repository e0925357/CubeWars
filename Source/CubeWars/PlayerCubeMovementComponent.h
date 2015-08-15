// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "PlayerCubeMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API UPlayerCubeMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UPlayerCubeMovementComponent();

	void move(float DeltaTime);
	
	void SetSpeed(float speed);
	float GetSpeed();

	void StartJitter();
	void StopJitter();

protected:
	float Speed;
	bool isJittering;
	FVector truePosition;
	FVector jitterOffset;
	float jitterTimer;
};
