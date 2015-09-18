// This source code file is protected by the MIT License (MIT)

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "PlayerCubeMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API UPlayerCubeMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UPlayerCubeMovementComponent();

	void move(float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = Movement)
	void SetSpeed(float speed);

	UFUNCTION(BlueprintCallable, Category = Movement)
	float GetSpeed();

	void StartJitter();
	void StopJitter();

protected:
	float Speed;
	bool isJittering;
	FVector truePosition;
	FVector jitterOffset;
	float jitterTimer;
};
