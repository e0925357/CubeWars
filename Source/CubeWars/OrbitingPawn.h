// This source code file is protected by the MIT License (MIT)

#pragma once

#include "GameFramework/Pawn.h"
#include "OrbitingPawn.generated.h"

UCLASS()
class CUBEWARS_API AOrbitingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOrbitingPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(NetMulticast, reliable)
	void SetupOrbitMulticast(const FVector& Center, float CamArmLength, float RotationSpeed);

	void SetupOrbit(const FVector& Center, float CamArmLength, float RotationSpeed);

private:
	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* RotatingMovementComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
};
