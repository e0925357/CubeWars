// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "PlayerCubeMovementComponent.h"


UPlayerCubeMovementComponent::UPlayerCubeMovementComponent() : Speed(150.0f)
{

}

void UPlayerCubeMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
	if(!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * Speed;
	if(!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
	}
};

void UPlayerCubeMovementComponent::SetSpeed(float speed)
{
	Speed = speed;
}

float UPlayerCubeMovementComponent::GetSpeed()
{
	return Speed;
}
