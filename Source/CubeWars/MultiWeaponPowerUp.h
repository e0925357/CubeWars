// This source code file is protected by the MIT License (MIT)

#pragma once

#include "WeaponPowerUp.h"
#include "MultiWeaponPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API AMultiWeaponPowerUp : public AWeaponPowerUp
{
	GENERATED_BODY()
public:

	AMultiWeaponPowerUp();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	int32 MaxProjectileCount;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	int32 MinProjectileCount;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float MaxPitchAngleDerivation;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float MaxYawAngleDerivation;

	virtual bool OnShoot_Implementation() override;
	
	
};
