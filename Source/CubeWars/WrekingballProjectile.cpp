// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "WrekingballProjectile.h"



void AWrekingballProjectile::PostHit(float DealtDamage)
{
	OnHitMulticast();

	damage -= DealtDamage;

	if(damage <= 0)
	{
		Destroy();
	}
}
