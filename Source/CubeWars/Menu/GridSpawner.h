// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GridSpawner.generated.h"

struct GridCubeSpawner
{
	GridCubeSpawner(float SpawnTimer, bool RightSide, FVector SpawnLocation)
		: SpawnTimer(SpawnTimer)
		, RightSide(RightSide)
		, SpawnLocation(SpawnLocation)
	{
	}

	float SpawnTimer;
	bool RightSide;
	FVector SpawnLocation;
};

UCLASS()
class CUBEWARS_API AGridSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridSpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** The class of the cube to be spawned */
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	TSubclassOf<AActor> GridCubeClass;
	
	UPROPERTY(EditDefaultsOnly, Category = Visual)
	int32 NumCubesPerSide;

	UPROPERTY(EditDefaultsOnly, Category = Visual)
	float RespawnTime;

	UPROPERTY(EditDefaultsOnly, Category = Visual)
	float GridCubeLifetime;

	UPROPERTY(EditDefaultsOnly, Category = Visual)
	float Dimension;

private:
	TArray<GridCubeSpawner> HorizontalSpawners;
	TArray<GridCubeSpawner> VerticalSpawners;

	void SpawnGridCube(const GridCubeSpawner& Spawner, bool Horizontal) const;

	void UpdateSpawner(GridCubeSpawner& Spawner, bool Horizontal, float DeltaTime);
};
