// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

class UObstacleMovementComponent;

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

	UObstacleMovementComponent* GetObstacleMovementComponent() { return movementComponent; };
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Components)
	UBoxComponent* BaseCollisionComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Components)
	UStaticMeshComponent* ObstacleVisual;
protected:
	UPROPERTY(VisibleAnywhere, Category = Components)
	UObstacleMovementComponent* movementComponent;
};
