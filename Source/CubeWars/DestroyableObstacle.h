// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CubeDebris.h"
#include "GameFramework/Actor.h"
#include "DestroyableObstacle.generated.h"

class UObstacleMovementComponent;
class UStaticMesh;
class UMaterialInstance;
class UStaticMeshComponent;

// Helper struct which represents the construction progress of a single part
struct PartConstructionProgress
{
	PartConstructionProgress(const FVector& StartPosition, const FVector& FinalPosition, UStaticMeshComponent* CubeVisual, UMaterialInstanceDynamic* MaterialInstance, float TotalTime)
		: StartPosition(StartPosition)
		, FinalPosition(FinalPosition)
		, CubeVisual(CubeVisual)
		, MaterialInstance(MaterialInstance)
		, TotalTime(TotalTime)
		, CurrentTime(0.0f)
	{
	}

	/** The initial position of the obstacle part (when CurrentTime = 0.0f) */
	FVector StartPosition;

	/** The final position of the obstacle part (when CurrentTime == TotalTime) */
	FVector FinalPosition;

	/** The visual component to be animated */
	UStaticMeshComponent* CubeVisual;

	/** The dynamic material instance of the cube visual */
	UMaterialInstanceDynamic* MaterialInstance;

	/** The total time till the part is in position */
	float TotalTime;

	/** The current time of the timer which is used to interpolate between the start and the final position */
	float CurrentTime;
};

UCLASS()
class CUBEWARS_API ADestroyableObstacle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestroyableObstacle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	//If the actor takes damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	/** Sets the index of this obstacle */
	void SetObstacleIndex(int NewObstacleIndex) { ObstacleIndex = NewObstacleIndex; }

	/** Queries whether the construction of the obstacle is in progress */
	bool IsConstructionInProgress() const { return (ConstructionTimer < TotalConstructionTime); }

	UObstacleMovementComponent* GetObstacleMovementComponent() { return MovementComponent; };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Components)
	UBoxComponent* BaseCollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float MinConstructionTime;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float MaxConstructionTime;

	UPROPERTY(EditDefaultsOnly, Category = Visual)
	float MinConstructionDistanceMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = Visual)
	float MaxConstructionDistanceMultiplier;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	int32 NumPartColumns;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	FVector Dimensions;

	UPROPERTY(EditDefaultsOnly, Category = Visual)
	float ExplosionForce;

protected:
	UFUNCTION(NetMulticast, reliable)
	void ClientDamageCallback(float NewHealth);

	UFUNCTION(NetMulticast, reliable)
	void ClientFallApart();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Stats)
	float MaxHealth;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = Stats)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACubeDebris> DebrisClass;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UObstacleMovementComponent* MovementComponent;

	float TotalConstructionTime;
	float ConstructionTimer;

	/** Index of the obstacle. Caution: This index only has the valid index on the server! */
	int ObstacleIndex;

	/** Material for the parts when their spawning is over */
	UMaterialInstance* FinalPartMaterial;

	// Parts of the Obstacle that can fall off
	TArray<UStaticMeshComponent*> FallOffParts;

	// Parts of the Obstacle that can only fall off when the Obstacle is destroyed
	TArray<UStaticMeshComponent*> FixedParts;

	// Array which holds the construction progress of the individual parts of the obstacle
	TArray<PartConstructionProgress> ConstructionProgresses;

private:
	UStaticMeshComponent* CreateObstaclePart(const FVector& RelativePosition, const FVector& Scale, UStaticMesh* StaticMesh, UMaterialInstance* MatInstance, int32 Number);

	void CreateDebris(UStaticMeshComponent* PartVisual);
};