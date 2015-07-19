// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeWars.h"
#include "DeathStarActorComponent.h"


// Sets default values for this component's properties
UDeathStarActorComponent::UDeathStarActorComponent() : MaxStartOffset(10.0f), AnimationSpeed(1.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionProfileName(TEXT("Pawn"));
	MeshComponent->SetSimulatePhysics(false);

	MeshComponent->AttachTo(this);
}


// Called when the game starts
void UDeathStarActorComponent::BeginPlay()
{
	Super::BeginPlay();

	Timer = ((FMath::Rand()%1024)/1024.0f)*MaxStartOffset;
	
	MeshComponent->SetStaticMesh(planeMesh);
	MeshComponent->SetMaterial(0, Material);
	MaterialInstance = UMaterialInstanceDynamic::Create(Material, MeshComponent);
	MeshComponent->SetMaterial(0, MaterialInstance);
}


// Called every frame
void UDeathStarActorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	Timer += DeltaTime * AnimationSpeed;
	MaterialInstance->SetScalarParameterValue(TEXT("Time"), Timer);
}

