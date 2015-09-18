// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "ObstacleMovementComponent.h"


UObstacleMovementComponent::UObstacleMovementComponent()
	: LeftMovementTarget(-100.f)
	, RightMovementTarget(100.0f)
	, MovementSpeed(100.0f)
	, MovingRight(true)
{
}

void UObstacleMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!UpdatedComponent)
	{
		return;
	}

	AActor* ActorOwner = UpdatedComponent->GetOwner();
	if (!ActorOwner || ActorOwner->IsPendingKill())
	{
		return;
	}

	FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation();

	if (MovingRight)
	{
		CurrentLocation.Y += MovementSpeed * DeltaTime;

		if (CurrentLocation.Y >= RightMovementTarget)
		{
			MovingRight = !MovingRight;
			CurrentLocation.Y = RightMovementTarget;
		}
	}
	else
	{
		CurrentLocation.Y -= MovementSpeed * DeltaTime;

		if (CurrentLocation.Y <= LeftMovementTarget)
		{
			MovingRight = !MovingRight;
			CurrentLocation.Y = LeftMovementTarget;
		}
	}

	FHitResult Hit(1.0f);
	MoveUpdatedComponent(CurrentLocation - UpdatedComponent->GetComponentLocation(), CurrentRotation, true, &Hit);

	// If we hit a trigger that destroyed us, abort.
	if (ActorOwner->IsPendingKill() || !UpdatedComponent)
	{
		return;
	}

	UpdateComponentVelocity();
}

void UObstacleMovementComponent::SetMovingRightMulticast_Implementation(bool NewMovingRight)
{
	MovingRight = NewMovingRight;
}