// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "Obstacle.h"
#include "Components/InterpToMovementComponent.h"
#include "ObstacleMovementComponent.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

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

	movementComponent = CreateDefaultSubobject<UObstacleMovementComponent>(TEXT("ObstacleMovement"));
	movementComponent->UpdatedComponent = RootComponent;
	

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


	
}