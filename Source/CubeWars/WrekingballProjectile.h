// Fill out your copyright notice in the Description page of Project Settings.

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
