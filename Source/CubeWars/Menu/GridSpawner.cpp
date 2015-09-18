// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "GridSpawner.h"

// Sets default values
AGridSpawner::AGridSpawner()
	: NumCubesPerSide(10)
	, RespawnTime(10.0f)
	, GridCubeLifetime(15.0f)
	, Dimension(100.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize the spawn lists
	const float HalfDimension = Dimension / 2.0f;
	const float DimensionStep = Dimension / (NumCubesPerSide - 1);
	const float ActorZCoord = GetActorLocation().Z;

	FRandomStream RandStream;

	for (int32 i = 0; i < NumCubesPerSide; ++i)
	{
		float CurrentOffset = -HalfDimension + (DimensionStep * i);
		bool RightSide = RandStream.RandRange(0, 1) != 0;

		GridCubeSpawner Spawner = GridCubeSpawner(0.0f, RightSide, FVector((RightSide ? HalfDimension : -HalfDimension), CurrentOffset, ActorZCoord));
		VerticalSpawners.Add(Spawner);
		SpawnGridCube(Spawner, false);

		RightSide = RandStream.RandRange(0, 1) != 0;
		Spawner = GridCubeSpawner(0.0f, RightSide, FVector(CurrentOffset, (RightSide ? HalfDimension : -HalfDimension), ActorZCoord));
		HorizontalSpawners.Add(Spawner);
		SpawnGridCube(Spawner, true);
	}
}

// Called every frame
void AGridSpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	for (int32 i = 0; i < NumCubesPerSide; ++i)
	{
		UpdateSpawner(HorizontalSpawners[i], true, DeltaTime);
		UpdateSpawner(VerticalSpawners[i], false, DeltaTime);
	}

}

void AGridSpawner::UpdateSpawner(GridCubeSpawner& Spawner, bool Horizontal, float DeltaTime)
{
	Spawner.SpawnTimer += DeltaTime;

	if (Spawner.SpawnTimer > RespawnTime)
	{
		SpawnGridCube(Spawner, Horizontal);
		Spawner.SpawnTimer -= RespawnTime;
	}
}

void AGridSpawner::SpawnGridCube(const GridCubeSpawner& Spawner, bool Horizontal) const
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		// spawn the projectile at the muzzle
		FRotator SpawnRotation = FRotator::ZeroRotator;

		if (Horizontal)
		{
			SpawnRotation = Spawner.RightSide ? FRotator(0.0f, 270.0f, 0.0f) : FRotator(0.0f, 90.0f, 0.0f);
		}
		else
		{
			if (Spawner.RightSide)
			{
				SpawnRotation = FRotator(0.0f, 180.0f, 0.0f);
			}
		}

		AActor* GridCube = World->SpawnActor<AActor>(GridCubeClass, Spawner.SpawnLocation, SpawnRotation);
		GridCube->SetLifeSpan(GridCubeLifetime);
	}
}