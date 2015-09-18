// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "Pickup.h"
#include "PowerUp.h"
#include "PlayerCube.h"
#include "PlayerCubeController.h"


// Sets default values
APickup::APickup() : RotationRate(90.0f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionProfileName(TEXT("PickUp"));
	MeshComponent->SetSimulatePhysics(false);

	RootComponent = MeshComponent;

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Component"));
	RotatingMovementComponent->bRotationInLocalSpace = true;
	RotatingMovementComponent->UpdatedComponent = RootComponent;
}

void APickup::BeginPlay()
{
	RotatingMovementComponent->RotationRate = FRotator(0.0f, RotationRate, 0.0f);
}

// Called every frame
void APickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if(PowerUp->IsValidLowLevel())
	{
		PowerUp->Lifetime -= DeltaTime;

		if(PowerUp->Lifetime <= 0)
		{
			Destroy();
		}
	}
}

//Sets the power-up for this pickup
void APickup::SetPowerUp_Implementation(int32 PowerUpNetTag)
{

	//Find the Actor
	for(TActorIterator<APowerUp> Iter(GetWorld(), APowerUp::StaticClass()); Iter; ++Iter)
	{
		APowerUp* pu = *Iter;

		if(pu->GetGUID() == PowerUpNetTag)
		{
			SetPowerUp(pu);
			break;
		}
	}
}

void APickup::SetPowerUp(APowerUp* PowerUp)
{
	this->PowerUp = PowerUp;

	if(PowerUp->IsValidLowLevel() && PowerUp->PickupMesh->IsValidLowLevel())
	{
		MeshComponent->SetStaticMesh(PowerUp->PickupMesh);

		if(PowerUp->PickupOverrideMaterial->IsValidLowLevel())
		{
			MeshComponent->SetMaterial(0, PowerUp->PickupOverrideMaterial);
		}
	}
}

//Returns the power-up containted in this pick-up
APowerUp* APickup::GetPowerUp()
{
	return PowerUp;
}

float APickup::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(EventInstigator->IsValidLowLevel() && PowerUp->IsValidLowLevel())
	{
		APlayerCube* PlayerCube = Cast<APlayerCube>(EventInstigator->GetPawn());

		if(PlayerCube != nullptr)
		{
			PlayerCube->SetPowerUp(PowerUp->GetGUID());
		}

		APlayerCubeController* playerCubeController = Cast<APlayerCubeController>(EventInstigator);

		if(playerCubeController != nullptr)
		{
			playerCubeController->showNotificationMessage(PowerUp->Name);
		}
	}

	Destroy();

	return DamageAmount;
}

int32 APickup::GetGUID()
{
	return GUID;
}

void APickup::SetGUID_Implementation(int32 GUID)
{
	this->GUID = GUID;
}
