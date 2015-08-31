// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "CubeDebris.h"


// Sets default values
ACubeDebris::ACubeDebris()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bReplicateMovement = true;

	// Create and position a mesh component so we can see where our cube is
	CubeVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	CubeVisual->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CubeVisual->SetCollisionProfileName(TEXT("Debris"));
	CubeVisual->SetSimulatePhysics(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/Meshes/SimpleCube.SimpleCube"));
	if(CubeVisualAsset.Succeeded())
	{
		CubeVisual->SetStaticMesh(CubeVisualAsset.Object);
		//The mesh is 20x20x20, ingame the player-cube is 100x100x100 => debris will be 30x30x30
		CubeVisual->SetWorldScale3D(FVector(1.5f));

		static ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("/Game/Materials/Quadratic.Quadratic"));

		if(Material.Succeeded())
		{
			UMaterialInstance* mat = (UMaterialInstance*)Material.Object;

			CubeVisual->SetMaterial(0, mat);
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Materials/Quadratic.Quadratic'!"));
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Meshes/SimpleCube.SimpleCube'!"));
	}

	RootComponent = CubeVisual;
}

// Called when the game starts or when spawned
void ACubeDebris::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeDebris::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

