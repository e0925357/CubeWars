// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "DeathStarActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUBEWARS_API UDeathStarActorComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDeathStarActorComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere, Category = Animation)
	float MaxStartOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float AnimationSpeed;

	UPROPERTY(EditAnywhere, Category = Animation)
	UMaterial* Material;

	UPROPERTY(EditAnywhere, Category = Animation)
	UStaticMesh* planeMesh;
	
private:
	float Timer;

	bool tick;
	
	UMaterialInstanceDynamic* MaterialInstance;
};
