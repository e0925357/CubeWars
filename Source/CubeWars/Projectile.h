// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CubeDebris.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class CUBEWARS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category = Stats)
	float damage;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UMaterialInterface* hitDecalMaterial;

	UFUNCTION(NetMulticast, reliable)
	void OnHitMulticast();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitBP();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACubeDebris> DebrisClass;

	void SetInstigator(AController* Instigator);

protected:

	UPROPERTY(VisibleAnywhere, Category = Components)
	class UProjectileMovementComponent* movementComponent;

	AController* Instigator;

	//---------------------------------------------------------------------------------------------
	// Components & Visuals
	//---------------------------------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* CubeVisual;
	
};
