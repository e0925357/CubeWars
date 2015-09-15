// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "CubeDebris.h"


// Sets default values
ACubeDebris::ACubeDebris()
	: CubeVisual(nullptr)
	, DissolveStartTime(3.0f)
	, DebrisColor(1.0f, 1.0f, 1.0f)
	, DissolveColor(0.4f, 0.4f, 10.0f, 1.0f)
	, Timer(0.0f)
	, DissolveStarted(false)
	, DissolveParticleSystemComponent(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;
	bReplicateMovement = false;

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

			FLinearColor PartColor;

			if (CubeVisual->GetMaterial(0)->GetVectorParameterValue("Base Color", PartColor))
			{
				DebrisColor = PartColor;
			}
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

	MaterialInstance->SetVectorParameterValue("Base Color", DebrisColor);
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
			//DissolveParticleSystemComponent->bAutoDestroy = true;

			const FBox ActorAABB = GetComponentsBoundingBox(true);
			const FVector AABBExtent = ActorAABB.GetExtent();

			DissolveParticleSystemComponent->SetVectorRandParameter("SpawnArea", AABBExtent, -AABBExtent);

			float ParticleSize = FMath::Max(2.0f, FMath::Min(AABBExtent.X, AABBExtent.Y) / 4.0f);
			DissolveParticleSystemComponent->SetVectorParameter("Size", FVector(ParticleSize));

			DissolveParticleSystemComponent->SetColorParameter("InitialColor", DissolveColor);

			DissolveStarted = true;
		}

		float NewOpacity = (Timer - DissolveStartTime) / (InitialLifeSpan - DissolveStartTime);
		MaterialInstance->SetScalarParameterValue("Opacity", 1.0f - NewOpacity);
	}
}

void ACubeDebris::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (DissolveParticleSystemComponent->IsValidLowLevel())
	{
		DissolveParticleSystemComponent->DestroyComponent();
	}
}

void ACubeDebris::SetDebrisColor(const FLinearColor& NewColor)
{
	DebrisColor = NewColor;
	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue("Base Color", DebrisColor);
	}
}

void ACubeDebris::SetDebrisColorMulticast_Implementation(const FLinearColor& NewColor)
{
	SetDebrisColor(NewColor);
}