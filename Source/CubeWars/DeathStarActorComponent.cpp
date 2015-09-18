// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "DeathStarActorComponent.h"


// Sets default values for this component's properties
UDeathStarActorComponent::UDeathStarActorComponent()
	: UStaticMeshComponent()
	, MaxStartOffset(10.0f)
	, AnimationSpeed(0.4f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionProfileName(TEXT("NoCollision"));
	SetSimulatePhysics(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Game/Meshes/Plane.Plane"));
	if(VisualAsset.Succeeded())
	{
		planeMesh = VisualAsset.Object;
		SetWorldScale3D(FVector(4.5f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Meshes/Plane.Plane'!"));
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Materials/DeathStar.DeathStar"));

	if(MaterialAsset.Succeeded())
	{
		Material = (UMaterial*)MaterialAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find '/Game/Materials/DeathStar.DeathStar'!"));
	}
}


// Called when the game starts
void UDeathStarActorComponent::BeginPlay()
{
	Super::BeginPlay();

	Timer = ((FMath::Rand()%1024)/1024.0f)*MaxStartOffset;
	
	SetStaticMesh(planeMesh);
	SetMaterial(0, Material);
	MaterialInstance = CreateDynamicMaterialInstance(0);
	SetMaterial(0, MaterialInstance);
	tick = true;
}


// Called every frame
void UDeathStarActorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if(tick)
	{
		Timer += DeltaTime * AnimationSpeed;
		MaterialInstance->SetScalarParameterValue(TEXT("Time"), Timer);
	}
}

