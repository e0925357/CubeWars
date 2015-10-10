// This source code file is protected by the MIT License (MIT)

#pragma once

#include "Components/AudioComponent.h"
#include "AdvancedAudioComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Audio, Common), meta = (BlueprintSpawnableComponent))
class CUBEWARS_API UAdvancedAudioComponent : public UAudioComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = Sound)
	float GetCurrentPlaybackTime();
	
	
};
