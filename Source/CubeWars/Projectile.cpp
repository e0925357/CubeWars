// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "Projectile.h"
#include "AdvProjectileMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile() : damage(9.0f), Instigator(nullptr), DebrisCount(9), ImpactSize(10.0f)
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

	movementComponent = CreateDefaultSubobject<UAdvProjectileMovementComponent>(TEXT("CubeMovement"));
	movementComponent->UpdatedComponent = RootComponent;
	movementComponent->InitialSpeed = 3000.f;
	movementComponent->MaxSpeed = 3000.f;
	movementComponent->bRotationFollowsVelocity = false;
	movementComponent->bShouldBounce = false;
	
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
		float dealtDamage = ApplyDamage(OtherActor, OtherComp, NormalImpulse, Hit);

		SpawnDebris();

		PostHit(dealtDamage);
	}

	if(OtherActor != nullptr && OtherActor->bCanBeDamaged && OtherComp != nullptr && hitDecalMaterial != nullptr)
	{
		FVector actorToProjectileVector = GetActorLocation() - OtherActor->GetActorLocation();
		UGameplayStatics::SpawnDecalAttached(hitDecalMaterial, FVector(ImpactSize, ImpactSize, actorToProjectileVector.Size()), OtherComp, NAME_None, GetActorLocation(), FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition);
	}
}

void AProjectile::OnHitMulticast_Implementation()
{
	OnHitBP();
}

void AProjectile::SetInstigator(AController* InstigatorController)
{
	Instigator = InstigatorController;
}

float AProjectile::ApplyDamage(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	return OtherActor->TakeDamage(damage, FDamageEvent(), Instigator, this);
}

void AProjectile::PostHit(float DealtDamage)
{
	OnHitMulticast();

	Destroy();
}

void AProjectile::SpawnDebris_Implementation()
{
	UWorld* const World = GetWorld();

	if(World != nullptr)
	{
		for(int i = 0; i < DebrisCount; i++)
		{
			FVector Impulse((FMath::Rand()%1000) - 500, (FMath::Rand()%1000) - 500, (FMath::Rand()%1000) - 500);
			Impulse.Normalize();

			// spawn the debris
			ACubeDebris* debris = World->SpawnActor<ACubeDebris>(DebrisClass, GetActorLocation() + Impulse, FRotator(0, 0, 0));

			if(debris != nullptr)
			{
				FLinearColor PartColor;

				if(CubeVisual->GetMaterial(0)->GetVectorParameterValue("Base Color", PartColor))
				{
					debris->SetDebrisColorMulticast(PartColor);
				}

				UPrimitiveComponent* primitive = Cast<UPrimitiveComponent>(debris);

				if(primitive != nullptr)
				{
					Impulse *= 5000;
					primitive->AddImpulse(Impulse);
				}
			}
		}
	}
}
