// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "PlayerCube.h"
#include "PowerUp.generated.h"

/**
 * A PowerUp can be collected via a Pickup. It alters a PlayerCube's stats and behaviours for a limited amount of time.
 */
UCLASS(Blueprintable, BlueprintType)
class CUBEWARS_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:
	APowerUp();

	/**
	* The name of the PowerUp, will be shown to the player.
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Powerup)
	FString Name;

	/**
	* The duration of the effect once picked up in seconds.
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Powerup)
	float EffectDuration;

	/**
	* The time the pick-up will be available in seconds.
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Pickup)
	float Lifetime;

	/**
	* The mesh the pick-up will use.
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Pickup)
	UStaticMesh* PickupMesh;
	
	/**
	* A material that will be used by the pickup insted of the default Material of the mesh.
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Pickup)
	UMaterialInterface* PickupOverrideMaterial;



	/**
	* @return the player cube that is currently using this powerup.
	*/
	UFUNCTION(BlueprintCallable, Category = Powerup)
	APlayerCube* GetPlayerCube();

	/**
	* @return the player cube that is currently using this powerup.
	*/
	UFUNCTION(BlueprintCallable, Category = Powerup)
	virtual bool IsAlive();

	/**
	* The PlayerCube will call this function once per tick.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = Powerup)
	void Tick(float DeltaTime);

	/**
	* The PlayerCube will call this function when it attaches the powerup to itself.
	* @param PlayerCube the player cube that now uses the powerup, never nullptr
	*/
	UFUNCTION(BlueprintNativeEvent, Category = Powerup)
	void OnAttach(APlayerCube* PlayerCube);

	/**
	* The PlayerCube will call this function when it detaches the powerup from itself.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = Powerup)
	void OnDetach();

	/**
	* This method calls OnDetach() on every involved machine.
	*/
	UFUNCTION(NetMulticast, reliable)
	void Detach();

	/**
	* The PlayerCube will call this function before it shoots. If this method returns true, the PlayerCube will assume that this method already spawned a shot.
	* @return true if this method spawned a shot
	*/
	UFUNCTION(BlueprintNativeEvent, Category = Powerup)
	bool OnShoot();

	/**
	* The PlayerCube will call this function before it handles the shooting on the client. If this method returns true, the PlayerCube will assume that this method already handled everything.
	* @return true if this method handled the client-side of the shooting
	*/
	UFUNCTION(BlueprintNativeEvent, Category = Powerup)
	bool OnShootClient();

	/**
	* The PlayerCube will call this function before it takes any damage. This method will then return the true damage it deals to the PlayerCube.
	* @return the damage the PlayerCube takes
	*/
	UFUNCTION(BlueprintNativeEvent, Category = Powerup)
	float OnTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	/**
	* The PlayerCube will call this function before it handles the damage on the client. If this method returns true, the PlayerCube will assume that this method already handled everything.
	* @return true if this method handled the client-side of the damage-taking
	*/
	UFUNCTION(BlueprintNativeEvent, Category = Powerup)
	bool OnTakeDamageClient(AActor* DamageCauser);


	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, reliable)
	void setPickupId(int32 PickupId);

	int32 GetGUID();

	UFUNCTION(NetMulticast, reliable)
	void SetGUID(int32 GUID);

private:
	UPROPERTY()
	APlayerCube* PlayerCube;
	int32 PickupId;
	int32 GUID;
};
