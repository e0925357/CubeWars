// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CubeDebris.generated.h"

UCLASS()
class CUBEWARS_API ACubeDebris : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeDebris();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	// Sets the new color for the debris and applies it to the material
	void SetDebrisColor(const FLinearColor& NewColor);

	// A multicast version of the SetDebrisColor method
	UFUNCTION(NetMulticast, reliable)
	void SetDebrisColorMulticast(const FLinearColor& NewColor);

	UPROPERTY(VisibleAnywhere, Category=Components)
	UStaticMeshComponent* CubeVisual;
	
	UPROPERTY(VisibleAnywhere, Category = Visual)
	float DissolveStartTime;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	FLinearColor DebrisColor;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	FLinearColor DissolveColor;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* DissolveParticleSystem;

private:
	UMaterialInstanceDynamic* MaterialInstance;
	float Timer;
	bool DissolveStarted;
	UParticleSystemComponent* DissolveParticleSystemComponent;
};
