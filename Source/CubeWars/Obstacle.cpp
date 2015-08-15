// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "Obstacle.h"


// Sets default values
AObstacle::AObstacle()
	: MovementSpeed(10.0f)
	, MovingRight(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BaseCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BaseCollisionComponent;
	BaseCollisionComponent->InitBoxExtent(FVector(10.0f));
	BaseCollisionComponent->SetCollisionProfileName(TEXT("Obstacle"));

	ObstacleVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	ObstacleVisual->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ObstacleVisual->SetCollisionProfileName(TEXT("Obstacle"));
	ObstacleVisual->SetSimulatePhysics(false);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> ObstacleVisualAsset(TEXT("/Game/Meshes/SimpleCube.SimpleCube"));
	//if (ObstacleVisualAsset.Succeeded())
	//{
	//	ObstacleVisual->SetStaticMesh(ObstacleVisualAsset.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Meshes/SimpleCube.SimpleCube'!"));
	//}

	ObstacleVisual->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector currentLocation = GetActorLocation();

	if (MovingRight)
	{
		currentLocation.Y += MovementSpeed * DeltaTime;
		
		if (currentLocation.Y >= RightMovementTarget)
		{
			MovingRight = !MovingRight;
			currentLocation.Y = RightMovementTarget;
		}
	}
	else
	{
		currentLocation.Y -= MovementSpeed * DeltaTime;

		if (currentLocation.Y <= LeftMovementTarget)
		{
			MovingRight = !MovingRight;
			currentLocation.Y = LeftMovementTarget;
		}
	}

	SetActorLocation(currentLocation);
}

