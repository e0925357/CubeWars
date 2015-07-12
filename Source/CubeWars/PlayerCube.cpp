// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "PlayerCube.h"


// Sets default values
APlayerCube::APlayerCube()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WarCubeComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("WarCube"));
	WarCubeComponent->SetCollisionProfileName(TEXT("Pawn"));
	WarCubeComponent->SetLockedAxis(EDOFMode::XZPlane);
	WarCubeComponent->InitBoxExtent(FVector(2, 2, 2));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("MaterialInstance'/Game/Materials/Quadratic.Quadratic'"));

	if(Material.Object != NULL)
	{
		UMaterialInstance* mat = (UMaterialInstance*)Material.Object;

		WarCubeComponent->SetMaterial(0, mat);
	}

	RootComponent = WarCubeComponent;
}

// Called when the game starts or when spawned
void APlayerCube::BeginPlay()
{
	Super::BeginPlay();
	
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

	//TODO: Add aiming

	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCube::OnStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerCube::OnStopFire);
}

void APlayerCube::MoveHorizontal(float value)
{
	if(value != 0)
	{
		static const FVector RightVector(1, 0, 0);
		AddMovementInput(RightVector, value);
	}
}

void APlayerCube::OnStartFire()
{

}

void APlayerCube::OnStopFire()
{

}
