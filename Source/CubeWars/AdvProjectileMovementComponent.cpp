// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "AdvProjectileMovementComponent.h"


UAdvProjectileMovementComponent::UAdvProjectileMovementComponent() : bPierces(false)
{

}


UAdvProjectileMovementComponent::EHandleBlockingHitResult UAdvProjectileMovementComponent::HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	AActor* ActorOwner = UpdatedComponent ? UpdatedComponent->GetOwner() : NULL;
	if(!CheckStillInWorld() || !ActorOwner || ActorOwner->IsPendingKill())
	{
		return EHandleBlockingHitResult::Abort;
	}

	if(bPierces)
	{
		SubTickTimeRemaining = 0;
		return EHandleBlockingHitResult::AdvanceNextSubstep;
	}

	HandleImpact(Hit, TimeTick, MoveDelta);

	if(ActorOwner->IsPendingKill() || HasStoppedSimulation())
	{
		return EHandleBlockingHitResult::Abort;
	}

	SubTickTimeRemaining = TimeTick * (1.f - Hit.Time);
	return EHandleBlockingHitResult::Deflect;
}
