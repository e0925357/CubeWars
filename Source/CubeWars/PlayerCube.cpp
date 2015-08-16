// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "PlayerCube.h"
#include "Projectile.h"
#include "PlayerCubeMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "CubeDeathController.h"
#include "math.h"


// Sets default values
APlayerCube::APlayerCube() : TurnRate(20.0f), Health(100.0f), ShootTimer(0.0f), ShootDelay(0.7f), IsShooting(false), raisingState(0), targetHeight(0)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Create and position a mesh component so we can see where our cube is
	CubeVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	CubeVisual->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CubeVisual->SetCollisionProfileName(TEXT("Pawn"));
	CubeVisual->SetSimulatePhysics(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/Meshes/SimpleCube.SimpleCube"));
	if(CubeVisualAsset.Succeeded())
	{
		CubeVisual->SetStaticMesh(CubeVisualAsset.Object);
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

	// Create a camera and attach to our spring arm
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	CubeMovement = CreateDefaultSubobject<UPlayerCubeMovementComponent>(TEXT("CubeMovement"));
	CubeMovement->UpdatedComponent = RootComponent;
	CubeMovement->SetSpeed(300.0f);

	//Prepare the death effect
	createDeathEffect(FVector(0, 0, 3.3333f), FRotator(0, 0, 0), TEXT("DeathEffect1"));
	createDeathEffect(FVector(0, 0, -3.3333f), FRotator(0, 0, 0), TEXT("DeathEffect2"));

	createDeathEffect(FVector(0, 3.3333f, 0), FRotator(0, 0, 90), TEXT("DeathEffect3"));
	createDeathEffect(FVector(0, -3.3333f, 0), FRotator(0, 0, 90), TEXT("DeathEffect4"));

	createDeathEffect(FVector(3.3333f, 0, 0), FRotator(90, 0, 0), TEXT("DeathEffect5"));
	createDeathEffect(FVector(-3.3333f, 0, 0), FRotator(90, 0, 0), TEXT("DeathEffect6"));
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
		//Handle jittering
		if(Role == ROLE_Authority && raisingState >= 1)
		{
			if(raisingState == 1)
			{
				if(GetActorLocation().Z >= targetHeight)
				{
					raisingState = 2;

					ACubeDeathController* deathController = Cast<ACubeDeathController>(GetController());

					if(deathController != nullptr)
					{
						deathController->actorReachedHeight();
					}
				}
				else
				{
					CubeMovement->AddInputVector(FVector::UpVector);
				}
			}

			static const float DeathRotationSpeed = PI*0.68438f;
			static const FVector direction(FMath::Sqrt(1.0f/3.0f), FMath::Sqrt(1.0f/3.0f), -FMath::Sqrt(1.0f/3.0f));

			AddActorWorldRotation(FQuat(direction, DeathRotationSpeed*DeltaTime));
		}

		//Move locally(prediction) and at the server
		CubeMovement->move(DeltaTime);

		if(Role == ROLE_Authority)
		{
			//Distribute the current values to the clients
			CurrentPosition = GetActorLocation();
			CurrentRotation = GetActorRotation();
		}
	}

	if(raisingState == 1)
	{
		DeathProgress = (GetActorLocation().Z - startHeight) / (targetHeight - startHeight);

		if(deathAudioComponent)
		{
			deathAudioComponent->SetPitchMultiplier(1 + DeathProgress);
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

	if(Role == ROLE_Authority)
	{
		//Handle shooting
		if(ShootTimer <= 0)
		{
			if(IsShooting)
			{
				ShootTimer = ShootDelay;
				Shoot();
			}
		}
		else
		{
			ShootTimer -= DeltaTime;
		}
	}
}

void APlayerCube::Shoot()
{
	// try and fire a projectile
	if(ProjectileClass != nullptr)
	{
		const FRotator SpawnRotation = GetActorRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector OffsetVector(120.0f, 0.0f, 0.0f);
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(OffsetVector);

		UWorld* const World = GetWorld();
		if(World != nullptr)
		{
			// spawn the projectile at the muzzle
			AProjectile* projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
	}

	ShootOnClient();
}

void APlayerCube::ShootOnClient_Implementation()
{
	// try and play the sound if specified
	if(FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
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
		IsShooting = false;

		GetController()->UnPossess();
		
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.OverrideLevel = GetLevel();
		SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save AI controllers into a map
		AController* NewController = GetWorld()->SpawnActor<AController>(ACubeDeathController::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnInfo);
		if(NewController != nullptr)
		{
			// if successful will result in setting this->Controller 
			// as part of possession mechanics
			NewController->Possess(this);
		}

		bReplicateMovement = true;
	}

	ClientDamageCallback(DamageAmount);

	return DamageAmount;
}

void APlayerCube::ClientDamageCallback_Implementation(float damageAmount)
{
	// try and play the sound if specified
	if(DamageSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation());
	}

	HealthChanged();
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
	return  true;
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

void APlayerCube::Turn(float value)
{
	if(value != 0)
	{
		TurnServer(value);
	}
}

bool APlayerCube::TurnServer_Validate(float value)
{
	return true;
}

void APlayerCube::TurnServer_Implementation(float value)
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

bool APlayerCube::OnStartFire_Validate()
{
	return true;
}

void APlayerCube::OnStartFire_Implementation()
{
	if(Health > 0)
	{
		IsShooting = true;
	}
}

bool APlayerCube::OnStopFire_Validate()
{
	return true;
}

void APlayerCube::OnStopFire_Implementation()
{
	if(Health > 0)
	{
		IsShooting = false;
	}
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

void APlayerCube::startRaising(float targetHeight)
{
	this->targetHeight = targetHeight;
	raisingState = 1;
	CubeMovement->StartJitter();
	CubeMovement->SetSpeed(20.0f);

	startRaising_Client();
}

void APlayerCube::startRaising_Client_Implementation()
{
	// If this line is enabled, then the raising only works with dedicated servers
	//if(Role == ROLE_Authority) return;

	auto DeathComponentArray = GetComponentsByTag(UDeathStarActorComponent::StaticClass(), TEXT("DeathEffect"));

	for(UActorComponent* ActorComponent : DeathComponentArray)
	{
		USceneComponent* SceneComponent = dynamic_cast<USceneComponent*>(ActorComponent);
		SceneComponent->SetVisibility(true, true);
		SceneComponent->SetActive(true);
	}

	DeathProgress = 0;
	startHeight = GetActorLocation().Z;

	if(DeathSound)
	{
		deathAudioComponent = UGameplayStatics::PlaySoundAttached(DeathSound, GetRootComponent());
		deathAudioComponent->bStopWhenOwnerDestroyed = true;
	}
}

void APlayerCube::warped_Implementation()
{
	if(DeathWarpSound)
	{
		float pitch = 0.5f + (FMath::Rand()%1024)/1024.0f * 0.5f;
		UGameplayStatics::PlaySoundAtLocation(this, DeathWarpSound, GetActorLocation(), 1.0f, pitch);
	}
}

void APlayerCube::createDeathEffect(const FVector& location, const FRotator& rotation, FName name)
{
	UDeathStarActorComponent* deathEffect = CreateDefaultSubobject<UDeathStarActorComponent>(name);
	deathEffect->ComponentTags.Add(TEXT("DeathEffect"));
	deathEffect->SetVisibility(false, true);
	deathEffect->SetActive(false);
	deathEffect->SetRelativeLocation(location);
	deathEffect->SetRelativeRotation(rotation);
	deathEffect->AttachTo(GetRootComponent());
}
