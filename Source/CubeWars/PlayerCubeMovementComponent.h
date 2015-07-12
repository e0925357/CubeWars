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

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	void SetSpeed(float speed);
	float GetSpeed();

protected:
	float Speed;
};
