// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "AudioDevice.h"
#include "ActiveSound.h"
#include "AdvancedAudioComponent.h"


float UAdvancedAudioComponent::GetCurrentPlaybackTime()
{
	FAudioDevice* AudioDevice = GetAudioDevice();

	if(AudioDevice == nullptr)
	{
		return 0.0f;
	}

	FActiveSound* ActiveSound = AudioDevice->FindActiveSound(this);

	if(ActiveSound == nullptr)
	{
		return 0.0f;
	}
	else
	{
		return ActiveSound->PlaybackTime;
	}
}

