// Fill out your copyright notice in the Description page of Project Settings.

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
