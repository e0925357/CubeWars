// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "OrbitingPawn.h"
#include "math.h"


// Sets default values
AOrbitingPawn::AOrbitingPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Center"));

	// Use a spring arm to give the camera smooth, natural-feeling motion.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->RelativeRotation = FRotator(-20.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bDoCollisionTest = false;

	// Create a camera and attach to our center
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Component"));
	RotatingMovementComponent->bRotationInLocalSpace = true;
	RotatingMovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void AOrbitingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrbitingPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AOrbitingPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	//Bind nothing
}

void AOrbitingPawn::SetupOrbitMulticast_Implementation(const FVector& Center, float CamArmLength, float RotationSpeed)
{
	SetupOrbit(Center, CamArmLength, RotationSpeed);
}

void AOrbitingPawn::SetupOrbit(const FVector& Center, float CamArmLength, float RotationSpeed)
{
	RotatingMovementComponent->RotationRate = FRotator(0.0f, RotationSpeed, 0.0f);
	RootComponent->SetWorldLocation(Center);
	SpringArm->TargetArmLength = CamArmLength;
}
