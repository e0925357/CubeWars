// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerCube.generated.h"

UCLASS()
class CUBEWARS_API APlayerCube : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCube();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Return custom movement
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	//If the actor takes damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	//Tells the cube to start rising into the air!
	void startRaising(float targetHeight);

protected:

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = Stats)
	float Health;

	UFUNCTION(BlueprintImplementableEvent)
	void HealthChanged();

	void createDeathEffect(const FVector& location, const FRotator& rotation, FName name);

	UFUNCTION(NetMulticast, reliable)
	void startRaising_Client();

	//---------------------------------------------------------------------------------------------
	// Shooting
	//---------------------------------------------------------------------------------------------

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** Sound to play each time we take damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* DamageSound;

	/** The delay between shots */
	UPROPERTY(EditAnywhere, Category = Stats)
	float ShootDelay;

	float ShootTimer;

	bool IsShooting;

	void Shoot();

	UFUNCTION(Client, reliable)
	void ShootOnClient();

	UFUNCTION(NetMulticast, reliable)
	void ClientDamageCallback(float damageAmount);

	//---------------------------------------------------------------------------------------------
	// Movement
	//---------------------------------------------------------------------------------------------
	void MoveHorizontal(float value);

	UFUNCTION(Server, WithValidation, reliable)
	void MoveHorizontalServer(float value);

	void Turn(float value);

	UFUNCTION(Server, WithValidation, reliable)
	void TurnServer(float value);

	UFUNCTION(Server, WithValidation, reliable)
	void OnStartFire();

	UFUNCTION(Server, WithValidation, reliable)
	void OnStopFire();

	FRotator InitinalRotation;

	UPROPERTY(EditAnywhere, Category = Stats)
	float TurnRate;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_PosChange)
	FVector CurrentPosition;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_RotChange)
	FRotator CurrentRotation;

	UFUNCTION()
	void OnRep_PosChange();

	UFUNCTION()
	void OnRep_RotChange();

	UPROPERTY(Replicated, VisibleAnywhere, Category = Components)
	class UPlayerCubeMovementComponent* CubeMovement;

	float targetHeight;
	uint8 raisingState;


	//---------------------------------------------------------------------------------------------
	// Components & Visuals
	//---------------------------------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* CubeVisual;
	
	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;
};
