// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "GridSpawner.h"

// Sets default values
AGridSpawner::AGridSpawner()
	: NumCubesPerSide(10)
	, RespawnTimeMin(9.0f)
	, RespawnTimeMax(10.0f)
	, GridCubeLifetime(15.0f)
	, Dimension(100.0f)
	, VerticalDimension(100.0f)
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

	RandStream.GenerateNewSeed();

	for (int32 i = 0; i < NumCubesPerSide; ++i)
	{
		float CurrentOffset = -HalfDimension + (DimensionStep * i);
		bool RightSide = RandStream.RandRange(0, 1) != 0;

		GridCubeSpawner Spawner = GridCubeSpawner(RandStream.FRandRange(0.0f, RespawnTimeMax), RightSide, FVector((RightSide ? HalfDimension : -HalfDimension), CurrentOffset, ActorZCoord));
		VerticalSpawners.Add(Spawner);

		RightSide = RandStream.RandRange(0, 1) != 0;
		Spawner = GridCubeSpawner(RandStream.FRandRange(0.0f, RespawnTimeMax), RightSide, FVector(CurrentOffset, (RightSide ? HalfDimension : -HalfDimension), ActorZCoord));
		HorizontalSpawners.Add(Spawner);
	}
}

// Called every frame
void AGridSpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	for(int32 i = 0; i < HorizontalSpawners.Num(); ++i)
	{
		UpdateSpawner(HorizontalSpawners[i], true, DeltaTime);
		UpdateSpawner(VerticalSpawners[i], false, DeltaTime);
	}

}

void AGridSpawner::UpdateSpawner(GridCubeSpawner& Spawner, bool Horizontal, float DeltaTime)
{
	Spawner.SpawnTimer -= DeltaTime;

	if (Spawner.SpawnTimer < 0.0f)
	{
		SpawnGridCube(Spawner, Horizontal);
		Spawner.SpawnTimer += RandStream.FRandRange(RespawnTimeMin, RespawnTimeMax);
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

		FVector SpawnPosition = Spawner.SpawnLocation;
		SpawnPosition.Z += RandStream.FRandRange(-VerticalDimension, VerticalDimension);

		AActor* GridCube = World->SpawnActor<AActor>(GridCubeClass, SpawnPosition, SpawnRotation);
		GridCube->SetLifeSpan(GridCubeLifetime);
	}
}