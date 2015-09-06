// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "CubeDebris.h"


// Sets default values
ACubeDebris::ACubeDebris()
	: DissolveStartTime(3.0f)
	, Timer(0.0f)
	, DissolveStarted(false)
	, DissolveParticleSystemComponent(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bReplicateMovement = true;
	PrimaryActorTick.bCanEverTick = true;

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

		static ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("/Game/Materials/QuadraticTransparent.QuadraticTransparent"));

		if(Material.Succeeded())
		{
			UMaterialInstance* mat = (UMaterialInstance*)Material.Object;

			CubeVisual->SetMaterial(0, mat);
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Materials/QuadraticTransparent.QuadraticTransparent'!"));
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Meshes/SimpleCube.SimpleCube'!"));
	}

	RootComponent = CubeVisual;

	// Load dissolve particle system
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("/Game/Effects/DissolveEffect.DissolveEffect"));
	if (ParticleSystem.Succeeded())
	{
		DissolveParticleSystem = ParticleSystem.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Effects/DissolveEffect.DissolveEffect'"));
	}


	InitialLifeSpan = 5.0f;

	
}

// Called when the game starts or when spawned
void ACubeDebris::BeginPlay()
{
	Super::BeginPlay();
	
	MaterialInstance = CubeVisual->CreateDynamicMaterialInstance(0);
	CubeVisual->SetMaterial(0, MaterialInstance);
}

// Called every frame
void ACubeDebris::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	Timer += DeltaTime;

	if (Timer >= DissolveStartTime)
	{
		if (!DissolveStarted)
		{
			// Play the particle effect
			DissolveParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DissolveParticleSystem, GetActorLocation(), FRotator::ZeroRotator);

			DissolveStarted = true;
		}

		float NewOpacity = (Timer - DissolveStartTime) / (InitialLifeSpan - DissolveStartTime);
		MaterialInstance->SetScalarParameterValue("Opacity", 1.0f - NewOpacity);
	}
}

void ACubeDebris::Destroyed()
{
	if (DissolveParticleSystemComponent)
	{
		DissolveParticleSystemComponent->Deactivate();
	}
}