// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DestroyableObstacle.generated.h"

class UObstacleMovementComponent;
class UStaticMesh;
class UMaterialInstance;

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

	//If the actor takes damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	UObstacleMovementComponent* GetObstacleMovementComponent() { return MovementComponent; };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Components)
	UBoxComponent* BaseCollisionComponent;

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

	UPROPERTY(VisibleAnywhere, Category = Components)
	UObstacleMovementComponent* MovementComponent;

	// Parts of the Obstacle that can fall off
	TArray<UStaticMeshComponent*> FallOffParts;

	// Parts of the Obstacle that can only fall off when the Obstacle is destroyed
	TArray<UStaticMeshComponent*> FixedParts;

private:
	UStaticMeshComponent* CreateObstaclePart(const FVector& RelativePosition, const FVector& Scale, UStaticMesh* StaticMesh, UMaterialInstance* MatInstance, int32 Number);

	void CreateDebris(UStaticMeshComponent* PartVisual);
};