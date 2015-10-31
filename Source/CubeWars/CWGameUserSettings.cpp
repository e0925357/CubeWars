// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "AudioDevice.h"
#include "CWGameUserSettings.h"
#include "CubeGameInstance.h"

namespace
{
	const float DEFAULT_SENSITIVITY_SCALE = 1.0f;
}

UCWGameUserSettings::UCWGameUserSettings()
	: UGameUserSettings()
	, MinPlayerTurnSensitivityScale(0.5f)
	, MaxPlayerTurnSensitivityScale(3.0f)
	, TurnSensitivity((DEFAULT_SENSITIVITY_SCALE - MinPlayerTurnSensitivityScale) / (MaxPlayerTurnSensitivityScale - MinPlayerTurnSensitivityScale)) // Calculate the player turn sensitivity so that we get an initial scale of DEFAULT_SENSITIVITY_SCALE
{
}

void UCWGameUserSettings::LoadSettings(bool bForceReload/* = false*/)
{
	Super::LoadSettings(bForceReload);

	UE_LOG(LogTemp, Warning, TEXT("Die Eule flog über die Settings."));
}

void UCWGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	ApplySoundSettings();

	Super::ApplySettings(bCheckForCommandLineOverrides);
}

void UCWGameUserSettings::ApplySoundSettings()
{
	FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();
	if(AudioDevice != nullptr)
	{
		for(TMap<USoundClass*, FSoundClassProperties>::TIterator It(AudioDevice->SoundClasses); It; ++It)
		{
			USoundClass* ThisSoundClass = It.Key();

			if(ThisSoundClass == nullptr)
			{
				continue;
			}

			if(ThisSoundClass->GetName().Find(TEXT("MasterSound")) != INDEX_NONE)
			{
				// the audiodevice function logspams for some reason
				//AudioDevice->SetClassVolume(ThisSoundClass, NewVolume);
				ThisSoundClass->Properties.Volume = MasterSoundVolume;
			}
			else if(ThisSoundClass->GetName().Find(TEXT("Effects")) != INDEX_NONE)
			{
				// the audiodevice function logspams for some reason
				//AudioDevice->SetClassVolume(ThisSoundClass, NewVolume);
				ThisSoundClass->Properties.Volume = EffectsSoundVolume;
			}
			else if(ThisSoundClass->GetName().Find(TEXT("Music")) != INDEX_NONE)
			{
				// the audiodevice function logspams for some reason
				//AudioDevice->SetClassVolume(ThisSoundClass, NewVolume);
				ThisSoundClass->Properties.Volume = MusicSoundVolume;
			}
			else if(ThisSoundClass->GetName().Find(TEXT("Voice")) != INDEX_NONE)
			{
				// the audiodevice function logspams for some reason
				//AudioDevice->SetClassVolume(ThisSoundClass, NewVolume);
				ThisSoundClass->Properties.Volume = VoiceSoundVolume;
			}
		}
	}
}

float UCWGameUserSettings::GetMasterSoundVolume() const
{
	return MasterSoundVolume;
}
void UCWGameUserSettings::SetMasterSoundVolume(float Volume)
{
	MasterSoundVolume = Volume;
}

float UCWGameUserSettings::GetEffectsSoundVolume() const
{
	return EffectsSoundVolume;
}
void UCWGameUserSettings::SetEffectsSoundVolume(float Volume)
{
	EffectsSoundVolume = Volume;
}

float UCWGameUserSettings::GetMusicSoundVolume() const
{
	return MusicSoundVolume;
}
void UCWGameUserSettings::SetMusicSoundVolume(float Volume)
{
	MusicSoundVolume = Volume;
}

float UCWGameUserSettings::GetVoiceSoundVolume() const
{
	return VoiceSoundVolume;
}
void UCWGameUserSettings::SetVoiceSoundVolume(float Volume)
{
	VoiceSoundVolume = Volume;
}

float UCWGameUserSettings::GetTurnSensitivity() const
{
	return TurnSensitivity;
}
void UCWGameUserSettings::SetTurnSensitivity(float NewTurnSensitivity)
{
	TurnSensitivity = NewTurnSensitivity;
}

float UCWGameUserSettings::GetPlayerTurnSensitivityScale() const
{
	return MinPlayerTurnSensitivityScale + (MaxPlayerTurnSensitivityScale - MinPlayerTurnSensitivityScale) * TurnSensitivity;
}