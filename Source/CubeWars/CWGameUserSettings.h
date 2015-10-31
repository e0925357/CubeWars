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
	UCWGameUserSettings();

	/** Loads the user settings from persistent storage */
	virtual void LoadSettings(bool bForceReload = false) override;

	/** Applies all current user settings to the game and saves to permanent storage (e.g. file), optionally checking for command line overrides. */
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

	void ApplySoundSettings();

	float GetMasterSoundVolume() const;
	void SetMasterSoundVolume(float Volume);

	float GetEffectsSoundVolume() const;
	void SetEffectsSoundVolume(float Volume);

	float GetMusicSoundVolume() const;
	void SetMusicSoundVolume(float Volume);

	float GetVoiceSoundVolume() const;
	void SetVoiceSoundVolume(float Volume);

	/** The value of the sensitivity slider in the options menu. Ranges from 0.0f to 1.0f */
	float GetTurnSensitivity() const;
	void SetTurnSensitivity(float NewTurnSensitivity);

	/** The PlayerTurnScale is the scale which is to be applied to the turning speed of the player */
	float GetPlayerTurnSensitivityScale() const;
	
protected:

	UPROPERTY(config)
	float MasterSoundVolume;

	UPROPERTY(config)
	float EffectsSoundVolume;

	UPROPERTY(config)
	float MusicSoundVolume;

	UPROPERTY(config)
	float VoiceSoundVolume;

	float MinPlayerTurnSensitivityScale;
	float MaxPlayerTurnSensitivityScale;

	UPROPERTY(config)
	float TurnSensitivity;
};
