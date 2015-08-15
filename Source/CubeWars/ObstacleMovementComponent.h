// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/MovementComponent.h"
#include "ObstacleMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API UObstacleMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	UObstacleMovementComponent();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float LeftMovementTarget;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float RightMovementTarget;

	/** The speed the obstacle is moving with  */
	UPROPERTY(EditAnywhere, Category = Gameplay)
	float MovementSpeed;

	/** true if the obstacle is moving right, false if moving left */
	UPROPERTY(EditAnywhere, Category = Gameplay)
	bool MovingRight;
};
