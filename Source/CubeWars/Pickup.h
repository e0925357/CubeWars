// This source code file is protected by the MIT License (MIT)

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class APowerUp;

UCLASS()
class CUBEWARS_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(EditDefaultsOnly)
	float RotationRate;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Sets the power-up for this pickup
	UFUNCTION(NetMulticast, reliable)
	void SetPowerUp(int32 PowerUpNetTag);

	//Sets the power-up for this pickup
	void SetPowerUp(class APowerUp* PowerUp);

	//Returns the power-up containted in this pick-up
	APowerUp* GetPowerUp();

	//If the actor takes damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	int32 GetGUID();

	UFUNCTION(NetMulticast, reliable)
	void SetGUID(int32 GUID);

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* RotatingMovementComponent;

private:
	APowerUp* PowerUp;
	int32 GUID;
	
};
