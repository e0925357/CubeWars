// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "PlayerCube.h"
#include "PlayerCubeMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerCube::APlayerCube() : TurnRate(20.0f), Health(100.0f)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

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

	if(Role == ROLE_Authority || IsLocallyControlled())
	{
		//Move locally(prediction) and at the server
		CubeMovement->move(DeltaTime);

		if(Role == ROLE_Authority)
		{
			//Distribute the current values to the clients
			CurrentPosition = GetActorLocation();
			CurrentRotation = GetActorRotation();
		}
	}

	if(IsLocallyControlled() && Role != ROLE_Authority)
	{
		//Perform prediction correction
		//Are we too far apart?
		FVector Diff = CurrentPosition - GetActorLocation();

		if(Diff.SizeSquared() > 100.0f)
		{
			//sweep to server location at 110% speed
			FVector CorrectionVector = Diff;
			CorrectionVector.Normalize();
			CorrectionVector *= CubeMovement->GetSpeed()*1.1f*DeltaTime;
			SetActorLocation(GetActorLocation() + CorrectionVector);
		}
	}
}

float APlayerCube::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(Health <= 0)
	{
		return 0;
	}

	Health -= DamageAmount;

	if(Health <= 0)
	{
		//TODO: Handle player death
	}

	return DamageAmount;
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

void APlayerCube::MoveHorizontal(float value)
{
	if(value != 0 && IsLocallyControlled() && Health > 0)
	{
		//Predict movement and notify server
		//Get right vector
		FVector RightVector(0, 1, 0);
		RightVector = InitinalRotation.RotateVector(RightVector);



		//Delegate movement to the MovementComponent
		CubeMovement->AddInputVector(RightVector * value);

		if(Role != ROLE_Authority)
		{
			//Notify server
			MoveHorizontalServer(value);
		}
	}
}

bool APlayerCube::MoveHorizontalServer_Validate(float value)
{
	return  Health > 0;
}

void APlayerCube::MoveHorizontalServer_Implementation(float value)
{
	if(value != 0 && Role == ROLE_Authority && Health > 0)
	{
		//Get right vector
		FVector RightVector(0, 1, 0);
		RightVector = InitinalRotation.RotateVector(RightVector);

		

		//Delegate movement to the MovementComponent
		CubeMovement->AddInputVector(RightVector * value);
	}
}

bool APlayerCube::Turn_Validate(float value)
{
	return Health > 0;
}

void APlayerCube::Turn_Implementation(float value)
{
	if(value != 0 && Role == ROLE_Authority && Health > 0)
	{
		//GetWorld()->GetDeltaSeconds()
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += TurnRate*GetWorld()->GetDeltaSeconds()*value;
		float yaw = Rotation.Yaw;

		if(InitinalRotation.Yaw < -90.0f && yaw > 90.0f)
		{
			yaw -= 360.0f;
		}

		if(yaw - InitinalRotation.Yaw + 45.0f < 0.0f)
		{
			Rotation.Yaw = InitinalRotation.Yaw - 45.0f;
		}
		else if(yaw - InitinalRotation.Yaw - 45.0f > 0.0f)
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

void APlayerCube::OnRep_PosChange()
{
	if(!IsLocallyControlled())
	{
		SetActorLocation(CurrentPosition);
	}
}


void APlayerCube::OnRep_RotChange()
{
	SetActorRotation(CurrentRotation);
}

UPawnMovementComponent* APlayerCube::GetMovementComponent() const
{
	return CubeMovement;
}

void APlayerCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCube, CubeMovement);
	DOREPLIFETIME(APlayerCube, CurrentPosition);
	DOREPLIFETIME(APlayerCube, CurrentRotation);
}