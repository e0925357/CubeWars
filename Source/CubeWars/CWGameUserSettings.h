// This source code file is protected by the MIT License (MIT)

#pragma once

#include "CWGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class CUBEWARS_API UCWGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	/** Loads the user settings from persistent storage */
	virtual void LoadSettings(bool bForceReload = false) override;

	/** Applies all current user settings to the game and saves to permanent storage (e.g. file), optionally checking for command line overrides. */
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

	void ApplySoundSettings();

	float GetMasterSoundVolume();
	void SetMasterSoundVolume(float Volume);

	float GetEffectsSoundVolume();
	void SetEffectsSoundVolume(float Volume);

	float GetMusicSoundVolume();
	void SetMusicSoundVolume(float Volume);

	float GetVoiceSoundVolume();
	void SetVoiceSoundVolume(float Volume);
	
protected:

	UPROPERTY(config)
	float MasterSoundVolume;

	UPROPERTY(config)
	float EffectsSoundVolume;

	UPROPERTY(config)
	float MusicSoundVolume;

	UPROPERTY(config)
	float VoiceSoundVolume;
};
