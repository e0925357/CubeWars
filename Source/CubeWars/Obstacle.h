// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class CUBEWARS_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Components)
	UBoxComponent* BaseCollisionComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Components)
	UStaticMeshComponent* ObstacleVisual;

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
