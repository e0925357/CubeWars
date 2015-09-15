// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerUp.h"
#include "Projectile.h"
#include "WeaponPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API AWeaponPowerUp : public APowerUp
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float FireCooldown;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	USoundBase* ShotShound;

	virtual bool OnShoot_Implementation() override;

	virtual bool OnShootClient_Implementation() override;

	virtual void OnAttach_Implementation(APlayerCube* PlayerCube) override;

	virtual void OnDetach_Implementation() override;

private:
	float oldFireCooldown;
};
