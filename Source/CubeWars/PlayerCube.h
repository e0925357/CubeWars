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

protected:

	void MoveHorizontal(float value);

	UFUNCTION(Server, WithValidation, unreliable)
	void MoveHorizontalServer(float value);

	UFUNCTION(Server, WithValidation, unreliable)
	void Turn(float value);

	void OnStartFire();

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

	UPROPERTY(VisibleAnywhere, Category = Components)
	UBoxComponent* CubeCollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* CubeVisual;

	UPROPERTY(Replicated, VisibleAnywhere, Category = Components)
	class UPlayerCubeMovementComponent* CubeMovement;
	
};
