// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile() : damage(9.0f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

	// Create and position a mesh component so we can see where our cube is
	CubeVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	CubeVisual->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CubeVisual->SetCollisionProfileName(TEXT("Projectile"));
	CubeVisual->SetSimulatePhysics(false);
	CubeVisual->SetEnableGravity(false);
	CubeVisual->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);		// set up a notification for when this component hits something blocking

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/Meshes/SimpleCube.SimpleCube"));
	if(CubeVisualAsset.Succeeded())
	{
		CubeVisual->SetStaticMesh(CubeVisualAsset.Object);

		static ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("/Game/Materials/DefaultShot.DefaultShot"));

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

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("CubeMovement"));
	movementComponent->UpdatedComponent = RootComponent;
	movementComponent->InitialSpeed = 3000.f;
	movementComponent->MaxSpeed = 3000.f;
	movementComponent->bRotationFollowsVelocity = false;
	movementComponent->bShouldBounce = true;

	// Die after 5 seconds by default
	InitialLifeSpan = 5.0f;

	hitDecalMaterial = nullptr;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor != nullptr && Role == ROLE_Authority)
	{
		OtherActor->TakeDamage(damage, FDamageEvent(), nullptr, this);

		UWorld* const World = GetWorld();

		if(World != nullptr)
		{
			for(int i = 0; i < 9; i++)
			{
				FVector Impulse((FMath::Rand()%1000) - 500, (FMath::Rand()%1000) - 500, (FMath::Rand()%1000) - 500);
				Impulse.Normalize();

				// spawn the debris
				AActor* debris = World->SpawnActor<AActor>(DebrisClass, GetActorLocation() + Impulse, FRotator(0, 0, 0));

				UPrimitiveComponent* primitive = Cast<UPrimitiveComponent>(debris);

				if(primitive != nullptr)
				{
					Impulse *= 5000;
					primitive->AddImpulse(Impulse);
				}
			}
		}

		OnHitMulticast();

		Destroy();
	}
}

void AProjectile::OnHitMulticast_Implementation()
{
	OnHitBP();
}
