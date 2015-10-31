// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "BpVideoSettingsLib.h"
#include "CWGameUserSettings.h"


// Get a list of screen resolutions supported by video adapter
// NOTE: This function needs "RHI" to be added to <Project>.Build.cs file!
bool UBpVideoSettingsLib::GetSupportedScreenResolutions(TArray<FString>& Resolutions)
{
	FScreenResolutionArray ResolutionsArray;

	if(RHIGetAvailableResolutions(ResolutionsArray, true))  // needs the "RHI" dependency
	{
		for(const FScreenResolutionRHI& Resolution : ResolutionsArray)
		{
			if(Resolution.Width < MIN_SCREEN_WIDTH || Resolution.Height < MIN_SCREEN_HEIGHT)
			{
				continue;
			}

			FString Str = FString::FromInt(Resolution.Width) + "x" + FString::FromInt(Resolution.Height);
			Resolutions.AddUnique(Str);
		}

		return true;
	}

	return false;  // failed to obtain screen resolutions
}


// Get currently set screen resolution
FString UBpVideoSettingsLib::GetScreenResolution()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return FString("");
	}

	FIntPoint Resolution = Settings->GetScreenResolution();
	return FString::FromInt(Resolution.X) + "x" + FString::FromInt(Resolution.Y);
}


// Check whether or not we are currently running in fullscreen mode
bool UBpVideoSettingsLib::IsInFullscreen()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return false;
	}

	return Settings->GetFullscreenMode() == EWindowMode::Fullscreen;
}


// Set the desired screen resolution (does not change it yet)
bool UBpVideoSettingsLib::SetScreenResolution(const int32 Width, const int32 Height, const bool Fullscreen)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return false;
	}

	Settings->SetScreenResolution(FIntPoint(Width, Height));
	Settings->SetFullscreenMode(Fullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed);
	return true;
}


// Change the current screen resolution
bool UBpVideoSettingsLib::ChangeScreenResolution(const int32 Width, const int32 Height, const bool Fullscreen)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return false;
	}

	EWindowMode::Type WindowMode = Fullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed;
	Settings->RequestResolutionChange(Width, Height, WindowMode, false);
	return true;
}


// Get the current video quality settings
bool UBpVideoSettingsLib::GetVideoQualitySettings(int32& AntiAliasing, int32& Effects, int32& PostProcess,
	int32& Resolution, int32& Shadow, int32& Texture, int32& ViewDistance)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return false;
	}

	AntiAliasing = Settings->ScalabilityQuality.AntiAliasingQuality;
	Effects = Settings->ScalabilityQuality.EffectsQuality;
	PostProcess = Settings->ScalabilityQuality.PostProcessQuality;
	Resolution = Settings->ScalabilityQuality.ResolutionQuality;
	Shadow = Settings->ScalabilityQuality.ShadowQuality;
	Texture = Settings->ScalabilityQuality.TextureQuality;
	ViewDistance = Settings->ScalabilityQuality.ViewDistanceQuality;
	return true;
}


// Set the quality settings (not applied nor saved yet)
bool UBpVideoSettingsLib::SetVideoQualitySettings(const int32 AntiAliasing, const int32 Effects, const int32 PostProcess,
	const int32 Resolution, const int32 Shadow, const int32 Texture, const int32 ViewDistance)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return false;
	}

	Settings->ScalabilityQuality.AntiAliasingQuality = AntiAliasing;
	Settings->ScalabilityQuality.EffectsQuality = Effects;
	Settings->ScalabilityQuality.PostProcessQuality = PostProcess;
	Settings->ScalabilityQuality.ResolutionQuality = Resolution;
	Settings->ScalabilityQuality.ShadowQuality = Shadow;
	Settings->ScalabilityQuality.TextureQuality = Texture;
	Settings->ScalabilityQuality.ViewDistanceQuality = ViewDistance;
	return true;
}


// Check whether or not we have vertical sync enabled
bool UBpVideoSettingsLib::IsVSyncEnabled()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return false;
	}

	return Settings->IsVSyncEnabled();
}


// Set the vertical sync flag
bool UBpVideoSettingsLib::SetVSyncEnabled(const bool VSync)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return false;
	}

	Settings->SetVSyncEnabled(VSync);
	return true;
}


// Confirm and save current video mode (resolution and fullscreen/windowed)
bool UBpVideoSettingsLib::SaveVideoModeAndQuality()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return false;
	}

	Settings->ConfirmVideoMode();
	Settings->ApplySettings(false);
	return true;
}


// Revert to original video settings
bool UBpVideoSettingsLib::RevertVideoMode()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if(!Settings)
	{
		return false;
	}

	Settings->RevertVideoMode();
	return true;
}

bool UBpVideoSettingsLib::GetSoundSettings(float& MasterVolume, float& EffectsVolume, float& MusicVolume, float& VoiceVolume)
{
	UCWGameUserSettings* Settings = GetCWGameUserSettings();

	if(Settings == nullptr)
	{
		return false;
	}

	MasterVolume = Settings->GetMasterSoundVolume();
	EffectsVolume = Settings->GetEffectsSoundVolume();
	MusicVolume = Settings->GetMusicSoundVolume();
	VoiceVolume = Settings->GetVoiceSoundVolume();

	return true;
}


bool UBpVideoSettingsLib::SetSoundSettings(const float MasterVolume/* = 1.0f*/, const float EffectsVolume/* = 0.75f*/, const float MusicVolume/* = 0.5f*/, const float VoiceVolume/* = 1.0f*/)
{
	UCWGameUserSettings* Settings = GetCWGameUserSettings();

	if(Settings == nullptr)
	{
		return false;
	}

	Settings->SetMasterSoundVolume(MasterVolume);
	Settings->SetEffectsSoundVolume(EffectsVolume);
	Settings->SetMusicSoundVolume(MusicVolume);
	Settings->SetVoiceSoundVolume(VoiceVolume);

	return true;
}


bool UBpVideoSettingsLib::GetGameplaySettings(float& TurnSensitivity)
{
	UCWGameUserSettings* Settings = GetCWGameUserSettings();

	if (Settings == nullptr)
	{
		return false;
	}

	TurnSensitivity = Settings->GetTurnSensitivity();

	return true;
}


bool UBpVideoSettingsLib::SetGameplaySettings(const float TurnSensitivity/* = 0.2f*/)
{
	UCWGameUserSettings* Settings = GetCWGameUserSettings();

	if (Settings == nullptr)
	{
		return false;
	}

	Settings->SetTurnSensitivity(TurnSensitivity);

	return true;
}


//---- PRIVATE METHODS -------------------------------------------------------------------------------

// Try to get the GameUserSettings object from the engine
UGameUserSettings* UBpVideoSettingsLib::GetGameUserSettings()
{
	if(GEngine != nullptr)
	{
		return GEngine->GameUserSettings;
	}

	return nullptr;
}

UCWGameUserSettings* UBpVideoSettingsLib::GetCWGameUserSettings()
{
	UGameUserSettings* Settings = GetGameUserSettings();

	if(Settings == nullptr)
	{
		return nullptr;
	}

	return Cast<UCWGameUserSettings>(Settings);
}

