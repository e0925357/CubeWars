// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "PlayerCubeMovementComponent.h"
#include "PlayerCube.h"


UPlayerCubeMovementComponent::UPlayerCubeMovementComponent() : Speed(150.0f), isJittering(false)
{

}

void UPlayerCubeMovementComponent::move(float DeltaTime)
{
	// Make sure that everything is still valid, and that we are allowed to move.
	if(!PawnOwner)
	{
		return;
	}

	if(isJittering)
	{
		PawnOwner->SetActorLocation(truePosition);
	}

	// Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * Speed;
	if(!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
	}

	if(isJittering)
	{
		truePosition = PawnOwner->GetActorLocation();

		//apply jitter
		if(jitterTimer > 0)
		{
			jitterTimer -= DeltaTime;
		}
		else
		{
			jitterTimer = ((FMath::Rand()%10000)/10000.0f)*0.5f + 0.1f;

			static const float Offset = 15;
			static const float DoubleOffset = Offset*2;

			jitterOffset.Set(((FMath::Rand()%10000)/10000.0f)*DoubleOffset - Offset, ((FMath::Rand()%10000)/10000.0f)*DoubleOffset - Offset, ((FMath::Rand()%10000)/10000.0f)*DoubleOffset - Offset);

			//Play sound
			APlayerCube* player = Cast<APlayerCube>(PawnOwner);

			if(player)
			{
				player->warped();
			}
		}

		PawnOwner->SetActorLocation(truePosition + jitterOffset);
	}
}

void UPlayerCubeMovementComponent::SetSpeed(float speed)
{
	Speed = speed;
}

float UPlayerCubeMovementComponent::GetSpeed()
{
	return Speed;
}

void UPlayerCubeMovementComponent::StartJitter()
{
	isJittering = true;
	jitterTimer = 0;

	if(!PawnOwner)
	{
		return;
	}

	truePosition = PawnOwner->GetActorLocation();
}

void UPlayerCubeMovementComponent::StopJitter()
{
	isJittering = false;
}
