// This source code file is protected by the MIT License (MIT)

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
