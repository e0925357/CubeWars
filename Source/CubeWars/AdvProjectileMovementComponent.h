// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/ProjectileMovementComponent.h"
#include "AdvProjectileMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API UAdvProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
	
	UAdvProjectileMovementComponent();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	bool bPierces;
	
	virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;
	
};
