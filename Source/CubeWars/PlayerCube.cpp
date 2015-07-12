// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "PlayerCube.h"
#include "PlayerCubeMovementComponent.h"


// Sets default values
APlayerCube::APlayerCube() : TurnRate(20.0f)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and position a mesh component so we can see where our cube is
	CubeVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	CubeVisual->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CubeVisual->SetCollisionProfileName(TEXT("Pawn"));
	CubeVisual->SetSimulatePhysics(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Meshes/SimpleCube.SimpleCube"));
	if(SphereVisualAsset.Succeeded())
	{
		CubeVisual->SetStaticMesh(SphereVisualAsset.Object);
		CubeVisual->SetWorldScale3D(FVector(5.0f));

		static ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("/Game/Materials/Quadratic.Quadratic"));

		if(Material.Succeeded())
		{
			UMaterialInstance* mat = (UMaterialInstance*)Material.Object;
			
			CubeVisual->SetMaterial(0, mat);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Materials/Quadratic.Quadratic'!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Meshes/SimpleCube.SimpleCube'!"));
	}

	RootComponent = CubeVisual;

	// Use a spring arm to give the camera smooth, natural-feeling motion.
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->RelativeRotation = FRotator(-20.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	SpringArm->AttachTo(RootComponent);

	// Create a camera and attach to our spring arm
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	CubeMovement = CreateDefaultSubobject<UPlayerCubeMovementComponent>(TEXT("CubeMovement"));
	CubeMovement->UpdatedComponent = RootComponent;
	CubeMovement->SetSpeed(300.0f);
}

// Called when the game starts or when spawned
void APlayerCube::BeginPlay()
{
	Super::BeginPlay();
	
	InitinalRotation = GetActorRotation();
}

// Called every frame
void APlayerCube::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APlayerCube::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveHorizontal", this, &APlayerCube::MoveHorizontal);
	InputComponent->BindAxis("Turn", this, &APlayerCube::Turn);

	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCube::OnStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerCube::OnStopFire);
}

bool APlayerCube::MoveHorizontal_Validate(float value)
{
	return true;
}

void APlayerCube::MoveHorizontal_Implementation(float value)
{
	if(value != 0)
	{
		//Get right vector
		FVector RightVector(0, 1, 0);
		RightVector = InitinalRotation.RotateVector(RightVector);

		

		//Delegate movement to the MovementComponent
		CubeMovement->AddInputVector(RightVector * value);
	}
}

void APlayerCube::Turn(float value)
{
	if(value != 0)
	{
		//GetWorld()->GetDeltaSeconds()
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += TurnRate*GetWorld()->GetDeltaSeconds()*value;

		if(Rotation.Yaw - InitinalRotation.Yaw + 45.0f < 0.0f)
		{
			Rotation.Yaw = InitinalRotation.Yaw - 45.0f;
		}
		else if(Rotation.Yaw - InitinalRotation.Yaw - 45.0f > 0.0f)
		{
			Rotation.Yaw = InitinalRotation.Yaw + 45.0f;
		}

		SetActorRotation(Rotation);
	}
}

void APlayerCube::OnStartFire()
{

}

void APlayerCube::OnStopFire()
{

}

UPawnMovementComponent* APlayerCube::GetMovementComponent() const
{
	return CubeMovement;
}
