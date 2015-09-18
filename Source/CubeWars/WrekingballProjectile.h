// This source code file is protected by the MIT License (MIT)

#pragma once

#include "Projectile.h"
#include "WrekingballProjectile.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API AWrekingballProjectile : public AProjectile
{
	GENERATED_BODY()
	
	
protected:

	virtual void PostHit(float DealtDamage) override;
	
};
