// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "PowerUp.h"
#include "Pickup.h"
#include "Net/UnrealNetwork.h"



APowerUp::APowerUp() : 
	Name(TEXT("Unnamed")),
	EffectDuration(5.0f),
	Lifetime(3.0f),
	PickupMesh(nullptr),
	PickupOverrideMaterial(nullptr),
	PlayerCube(nullptr),
	PickupId(-1)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void APowerUp::BeginPlay()
{
	if(PickupId != -1)
	{
		//Find the Actor
		for(TActorIterator<APickup> Iter(GetWorld(), APickup::StaticClass()); Iter; ++Iter)
		{
			APickup* pickup = *Iter;

			if(pickup->GetGUID() == PickupId)
			{
				pickup->SetPowerUp(this);
				break;
			}
		}
	}

	MaxEffectDuration = EffectDuration;
}

APlayerCube* APowerUp::GetPlayerCube()
{
	return PlayerCube;
}

bool APowerUp::IsAlive()
{
	return PlayerCube != nullptr && EffectDuration > 0.0f;
}

void APowerUp::Tick_Implementation(float DeltaTime)
{
	EffectDuration -= DeltaTime;
}

void APowerUp::OnAttach_Implementation(APlayerCube* PlayerCube)
{
	this->PlayerCube = PlayerCube;
}

void APowerUp::OnDetach_Implementation()
{
	PlayerCube = nullptr;
}

void APowerUp::Detach_Implementation()
{
	OnDetach();
}

bool APowerUp::OnShoot_Implementation()
{
	return false;
}

bool APowerUp::OnShootClient_Implementation()
{
	return false;
}

float APowerUp::OnTakeDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return DamageAmount;
}

bool APowerUp::OnTakeDamageClient_Implementation(AActor* DamageCauser)
{
	return false;
}

void APowerUp::setPickupId_Implementation(int32 PickupId)
{
	this->PickupId = PickupId;

	if(PickupId != -1)
	{
		//Find the Actor
		for(TActorIterator<APickup> Iter(GetWorld(), APickup::StaticClass()); Iter; ++Iter)
		{
			APickup* pickup = *Iter;

			if(pickup->GetGUID() == PickupId)
			{
				pickup->SetPowerUp(this);
				break;
			}
		}
	}
}

int32 APowerUp::GetGUID()
{
	return GUID;
}

void APowerUp::SetGUID_Implementation(int32 GUID)
{
	this->GUID = GUID;
}

float APowerUp::GetEffectDurationPercent()
{
	return FMath::Max(0.0f, EffectDuration)/MaxEffectDuration;
}

void APowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerUp, EffectDuration);
}

