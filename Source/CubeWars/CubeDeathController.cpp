// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "CubeDeathController.h"
#include "PlayerCube.h"
#include "CubeDebris.h"


ACubeDeathController::ACubeDeathController() : targetHeight(300), breakTime(2), breakTimer(0), waiting(false), explosionForce(5000)
{
	
}

void ACubeDeathController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	breakTimer = breakTime;
}

void ACubeDeathController::actorReachedHeight()
{
	waiting = true;
}

void ACubeDeathController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if(waiting)
	{
		if(breakTimer >= 0)
		{
			breakTimer -= DeltaTime;
		}
		else
		{
			//Spawn little cubes
			UWorld* const World = GetWorld();
			FRotator rot = GetPawn()->GetActorRotation();
			FActorSpawnParameters SpawnInfo;
			FVector Location;
			FVector Impulse;
			FVector ActorLocation = GetPawn()->GetActorLocation();

			for(float x = -33.3333f; x <= 33.3333f; x += 33.3333f)
			{
				for(float y = -33.3333f; y <= 33.3333f; y += 33.3333f)
				{
					for(float z = -33.3333f; z <= 33.3333f; z += 33.3333f)
					{
						Location.Set(x + ActorLocation.X, y + ActorLocation.Y, z + ActorLocation.Z);
						Impulse.Set(x, y, z);
						Impulse.Normalize();
						Impulse *= explosionForce;

						ACubeDebris* debris = World->SpawnActor<ACubeDebris>(ACubeDebris::StaticClass(), Location, rot, SpawnInfo);

						debris->CubeVisual->AddImpulse(Impulse);
					}
				}
			}

			//Destroy actor
			GetPawn()->Destroy();
		}
	}
}

void ACubeDeathController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	playerCube = Cast<APlayerCube>(InPawn);

	if(playerCube != nullptr)
	{
		//tell cube to rise
		playerCube->startRaising(targetHeight);
	}
}