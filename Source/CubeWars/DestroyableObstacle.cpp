// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "DestroyableObstacle.h"
#include "CubeDebris.h"
#include "ObstacleMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "CubeWarsGameMode.h"

namespace {
	const FVector CUBE_DIMENSIONS(20.0f, 20.0f, 20.0f);
	const int32 NUM_PARTS_PER_COLUMN = 4;
}

// Sets default values
ADestroyableObstacle::ADestroyableObstacle()
	: MinConstructionTime(0.5f)
	, MaxConstructionTime(1.0f)
	, MinConstructionDistanceMultiplier(2.0f)
	, MaxConstructionDistanceMultiplier(3.0f)
	, NumPartColumns(5)
	, Dimensions(20.0f, 300.0f, 50.0f)
	, ExplosionForce(5000.0f)
	, MaxHealth(26.0f)
	, Health(MaxHealth)
	, TotalConstructionTime(MaxConstructionTime)
	, ConstructionTimer(0.0f)
	, FinalPartMaterial(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Basic setup
	bReplicates = true;
	bReplicateMovement = true;

	BaseCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BaseCollisionComponent;
	//BaseCollisionComponent->InitBoxExtent(FVector(10.0f));
	BaseCollisionComponent->SetBoxExtent(Dimensions / 2.0f, true);
	BaseCollisionComponent->SetCollisionProfileName(TEXT("Obstacle"));
	BaseCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BaseCollisionComponent->SetSimulatePhysics(false);

	// Add the visual representations
	//The mesh is 20x20x20 -> match the parts to be the size of Dimensions in total
	const FVector DIMENSION_RATIO = Dimensions / CUBE_DIMENSIONS;
	const float CENTER_WIDTH = Dimensions.Y - (Dimensions.Y / NumPartColumns);
	const float X_OFFSET = Dimensions.X / 4.0f;
	const float Z_OFFSET = Dimensions.Z / 4.0f;

	FVector PartScale(DIMENSION_RATIO.X / 2.0f, (Dimensions.Y / NumPartColumns) / CUBE_DIMENSIONS.Y, DIMENSION_RATIO.Z / 2.0f);

	{
		// Create and position a mesh component so we can see where our cube is
		static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/Meshes/SimpleCube.SimpleCube"));
		if (CubeVisualAsset.Succeeded())
		{
			static ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("/Game/Materials/QuadraticTransparent.QuadraticTransparent"));

			if (Material.Succeeded())
			{
				for (int32 i = 0; i < NumPartColumns; ++i)
				{
					const float CENTER_Y = CENTER_WIDTH * (i / static_cast<float>(NumPartColumns - 1)) - CENTER_WIDTH / 2.0f;

					TArray<UStaticMeshComponent*> NewVisuals;

					NewVisuals.Add(CreateObstaclePart(FVector(X_OFFSET, CENTER_Y, Z_OFFSET), PartScale, CubeVisualAsset.Object, Material.Object, i * NUM_PARTS_PER_COLUMN));
					NewVisuals.Add(CreateObstaclePart(FVector(X_OFFSET, CENTER_Y, -Z_OFFSET), PartScale, CubeVisualAsset.Object, Material.Object, i * NUM_PARTS_PER_COLUMN + 1));
					NewVisuals.Add(CreateObstaclePart(FVector(-X_OFFSET, CENTER_Y, -Z_OFFSET), PartScale, CubeVisualAsset.Object, Material.Object, i * NUM_PARTS_PER_COLUMN + 2));
					NewVisuals.Add(CreateObstaclePart(FVector(-X_OFFSET, CENTER_Y, Z_OFFSET), PartScale, CubeVisualAsset.Object, Material.Object, i * NUM_PARTS_PER_COLUMN + 3));

					FRandomStream RandStream;
					RandStream.GenerateNewSeed();

					// First determine the parts that can fall off
					int32 Index = RandStream.RandRange(0, 3);
					FallOffParts.Add(NewVisuals[Index]);
					NewVisuals.RemoveAt(Index);

					Index = RandStream.RandRange(0, 2);
					FallOffParts.Add(NewVisuals[Index]);
					NewVisuals.RemoveAt(Index);

					Index = RandStream.RandRange(0, 1);
					FallOffParts.Add(NewVisuals[Index]);
					NewVisuals.RemoveAt(Index);

					// Now the fixed parts
					FixedParts.Append(NewVisuals);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Materials/QuadraticTransparent.QuadraticTransparent'!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Meshes/SimpleCube.SimpleCube'!"));
		}
	}

	// Load the material FinalPartMaterial
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("/Game/Materials/Quadratic.Quadratic"));
	if (Material.Succeeded())
	{
		FinalPartMaterial = Material.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Materials/Quadratic.Quadratic'!"));
	}

	// Movement component
	MovementComponent = CreateDefaultSubobject<UObstacleMovementComponent>(TEXT("ObstacleMovement"));
	MovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void ADestroyableObstacle::BeginPlay()
{
	Super::BeginPlay();

	TotalConstructionTime = MaxConstructionTime;

	// Initialize the construction structs for the parts of the obstacle
	FRandomStream Rand;
	for (int32 i = 0; i < ConstructionProgresses.Num(); ++i)
	{
		PartConstructionProgress& PartProgress = ConstructionProgresses[i];
		PartProgress.StartPosition = PartProgress.FinalPosition * Rand.FRandRange(MinConstructionDistanceMultiplier, MaxConstructionDistanceMultiplier);
		PartProgress.TotalTime = Rand.FRandRange(MinConstructionTime, MaxConstructionTime);

		UMaterialInstanceDynamic* MaterialInstance = PartProgress.CubeVisual->CreateDynamicMaterialInstance(0);
		PartProgress.CubeVisual->SetMaterial(0, MaterialInstance);
		PartProgress.MaterialInstance = MaterialInstance;
		MaterialInstance->SetScalarParameterValue("Opacity", 0.0f);
	}
}

// Called every frame
void ADestroyableObstacle::Tick( float DeltaTime )
{
	if(!this->IsValidLowLevel() || IsActorBeingDestroyed())
		return;

	Super::Tick( DeltaTime );

	for (int32 i = 0; i < ConstructionProgresses.Num();)
	{
		PartConstructionProgress& PartProgress = ConstructionProgresses[i];
		PartProgress.CurrentTime += DeltaTime;

		UStaticMeshComponent* CubeVisual = PartProgress.CubeVisual;
		UMaterialInstanceDynamic* MaterialInstance = PartProgress.MaterialInstance;
		FVector NewPosition;
		float NewOpacity;

		if (PartProgress.CurrentTime >= PartProgress.TotalTime)
		{
			NewPosition = PartProgress.FinalPosition;
			NewOpacity = 1.0f;
			ConstructionProgresses.RemoveAt(i);

			if (FinalPartMaterial)
			{
				CubeVisual->SetMaterial(0, FinalPartMaterial);
			}
		}
		else
		{
			float Delta = PartProgress.CurrentTime / PartProgress.TotalTime;
			NewPosition = FMath::Lerp(PartProgress.StartPosition, PartProgress.FinalPosition, Delta);
			NewOpacity = Delta;
			++i;
		}

		CubeVisual->SetRelativeLocation(NewPosition);

		if (MaterialInstance)
		{
			MaterialInstance->SetScalarParameterValue("Opacity", NewOpacity);
		}

	}
}

void ADestroyableObstacle::Destroyed()
{
	ACubeWarsGameMode* cwgm = GetWorld()->GetAuthGameMode<ACubeWarsGameMode>();
	if (cwgm)
	{
		cwgm->ObstacleDied(ObstacleIndex);
	}
}

float ADestroyableObstacle::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsConstructionInProgress())
	{
		if (Health <= 0)
		{
			return 0;
		}

		Health -= DamageAmount;



		if (Health <= 0)
		{
			// Let all the parts fly around and despawn the actor
			ClientFallApart();

			Destroy();
		}
		else
		{
			// Let some parts fly around
			ClientDamageCallback(Health);
		}

		return DamageAmount;
	}

	return 0.0f;
}

UStaticMeshComponent* ADestroyableObstacle::CreateObstaclePart(const FVector& RelativePosition, const FVector& Scale, UStaticMesh* StaticMesh, UMaterialInstance* MatInstance, int32 Number)
{
	FString VisualName("VisualRepresentation");
	VisualName.AppendInt(Number);

	UStaticMeshComponent* CubeVisual = CreateDefaultSubobject<UStaticMeshComponent>(FName(*VisualName));
	CubeVisual->AttachTo(RootComponent);
	CubeVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CubeVisual->SetCollisionProfileName(TEXT("Obstacle"));
	CubeVisual->SetSimulatePhysics(false);
	CubeVisual->SetRelativeLocation(RelativePosition);
	CubeVisual->SetStaticMesh(StaticMesh);
	CubeVisual->SetWorldScale3D(Scale);
	CubeVisual->SetMaterial(0, MatInstance);

	// Initialize the struct for the construction of th obstacle (some values (FinalPosition and TotalTime) will be recalculated, in BeginPlay, when the values from the blueprint are known)
	PartConstructionProgress PartConstruction(RelativePosition, RelativePosition, CubeVisual, nullptr, 1.0f);
	ConstructionProgresses.Add(PartConstruction);

	return CubeVisual;
}

void ADestroyableObstacle::ClientDamageCallback_Implementation(float NewHealth)
{
	if (!IsRunningDedicatedServer())
	{
		const int32 NUM_TOTAL_FALLOFF_PARTS = ((NumPartColumns * NUM_PARTS_PER_COLUMN) * 3) / 4;
		int32 RemainingParts = static_cast<int32>(NUM_TOTAL_FALLOFF_PARTS * (NewHealth / MaxHealth));

		int32 NumPartsToFallOff = FMath::Max(0, FallOffParts.Num() - RemainingParts);

		FRandomStream RandStream;
		RandStream.GenerateNewSeed();

		for (int32 i = 0; i < NumPartsToFallOff; ++i)
		{
			int32 Index = RandStream.RandRange(0, FallOffParts.Num() - 1);

			UStaticMeshComponent* PartVisual = FallOffParts[Index];
			FallOffParts.RemoveAt(Index);
			
			CreateDebris(PartVisual);
		}
	}
}

void ADestroyableObstacle::ClientFallApart_Implementation()
{
	for (int32 i = 0; i < FallOffParts.Num(); ++i)
	{
		UStaticMeshComponent* PartVisual = FallOffParts[i];
		CreateDebris(PartVisual);
	}
	FallOffParts.Empty();

	for (int32 i = 0; i < FixedParts.Num(); ++i)
	{
		UStaticMeshComponent* PartVisual = FixedParts[i];
		CreateDebris(PartVisual);
	}
	FixedParts.Empty();
}

void ADestroyableObstacle::CreateDebris(UStaticMeshComponent* PartVisual)
{
	// Destroy the component...
	UWorld* const World = GetWorld();
	PartVisual->DestroyComponent();

	// ...and spawn the actors instead
	FVector Impulse;
	const FVector& ActorLocation = GetActorLocation();
	FVector Location(ActorLocation.X + PartVisual->RelativeLocation.X, ActorLocation.Y + PartVisual->RelativeLocation.Y, ActorLocation.Z + PartVisual->RelativeLocation.Z);
	FActorSpawnParameters SpawnInfo;

	Impulse = PartVisual->RelativeLocation;
	Impulse.Normalize();
	Impulse *= ExplosionForce;

	// Location and Rotation is not correctly set here, but getting the location and rotation from the component in world space does not work?
	ACubeDebris* debris = World->SpawnActor<ACubeDebris>(DebrisClass, Location, GetActorRotation(), SpawnInfo);
	debris->SetReplicates(false);
	debris->SetActorScale3D(PartVisual->GetComponentScale());

	debris->CubeVisual->AddImpulse(Impulse);

	FLinearColor PartColor;

	if (PartVisual->GetMaterial(0)->GetVectorParameterValue("Base Color", PartColor))
	{
		debris->SetDebrisColor(PartColor);
	}
}

void ADestroyableObstacle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADestroyableObstacle, Health);
}