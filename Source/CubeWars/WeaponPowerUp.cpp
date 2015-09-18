// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "WeaponPowerUp.h"

bool AWeaponPowerUp::OnShoot_Implementation()
{
	// try and fire a projectile
	if(ProjectileClass != nullptr)
	{
		const FRotator SpawnRotation = GetPlayerCube()->GetActorRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector OffsetVector(120.0f, 0.0f, 0.0f);
		const FVector SpawnLocation = GetPlayerCube()->GetActorLocation() + SpawnRotation.RotateVector(OffsetVector);

		UWorld* const World = GetWorld();
		if(World != nullptr)
		{
			// spawn the projectile at the muzzle
			AProjectile* projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);

			projectile->SetInstigator(GetPlayerCube()->GetController());

			return true;
		}
	}

	return false;
}

bool AWeaponPowerUp::OnShootClient_Implementation()
{
	if(ShotShound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShotShound, GetPlayerCube()->GetActorLocation());
		return true;
	}

	return false;
}

void AWeaponPowerUp::OnAttach_Implementation(APlayerCube* PlayerCube)
{
	Super::OnAttach_Implementation(PlayerCube);

	oldFireCooldown = GetPlayerCube()->GetShootDelay();
	GetPlayerCube()->SetShootDelay(FireCooldown);
}

void AWeaponPowerUp::OnDetach_Implementation()
{
	GetPlayerCube()->SetShootDelay(oldFireCooldown);

	Super::OnDetach_Implementation();
}


