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

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float damage;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	UMaterialInterface* hitDecalMaterial;

	UFUNCTION(NetMulticast, reliable)
	void OnHitMulticast();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitBP();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ACubeDebris> DebrisClass;

	//The number of debris-actors to spawn upon death
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	int32 DebrisCount;

	UFUNCTION(BlueprintCallable, Category = Projectile)
	void SetInstigator(AController* InstigatorController);

protected:

	virtual float ApplyDamage(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, reliable)
	void SpawnDebris();

	virtual void PostHit(float DealtDamage);

	UPROPERTY(VisibleAnywhere, Category = Components)
	class UAdvProjectileMovementComponent* movementComponent;

	AController* Instigator;

	//---------------------------------------------------------------------------------------------
	// Components & Visuals
	//---------------------------------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* CubeVisual;
	
};
