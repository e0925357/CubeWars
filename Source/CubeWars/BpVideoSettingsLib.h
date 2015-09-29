// This source code file is protected by the MIT License (MIT)

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BpVideoSettingsLib.generated.h"

#define MIN_SCREEN_WIDTH 1024
#define MIN_SCREEN_HEIGHT 768

/**
 * Custom Blueprint Function Library.
 * Sources by impetus games, see https://impetus-games.com/blog/Persistent-Graphics-Settings-in-UE4?utm_content=bufferc36c7&utm_medium=social&utm_source=twitter.com&utm_campaign=buffer
 */
UCLASS()
class CUBEWARS_API UBpVideoSettingsLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Get a list of screen resolutions supported on this machine
	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static bool GetSupportedScreenResolutions(TArray<FString>& Resolutions);

	// Get currently set screen resolution
	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static FString GetScreenResolution();

	// Check whether or not we are currently running in fullscreen mode
	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static bool IsInFullscreen();

	// Set the desired screen resolution (does not change it yet)
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetScreenResolution(const int32 Width, const int32 Height, const bool Fullscreen);

	// Change the current screen resolution
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool ChangeScreenResolution(const int32 Width, const int32 Height, const bool Fullscreen);

	// Get the current video quality settings
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool GetVideoQualitySettings(int32& AntiAliasing, int32& Effects, int32& PostProcess, int32& Resolution, int32& Shadow, int32& Texture, int32& ViewDistance);

	// Set the quality settings (not applied nor saved yet)
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetVideoQualitySettings(const int32 AntiAliasing = 3, const int32 Effects = 3, const int32 PostProcess = 3,
		const int32 Resolution = 100, const int32 Shadow = 3, const int32 Texture = 3, const int32 ViewDistance = 3);

	// Check whether or not we have vertical sync enabled
	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static bool IsVSyncEnabled();

	// Set the vertical sync flag
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetVSyncEnabled(const bool VSync);

	// Confirm and save current video mode (resolution and fullscreen/windowed) as well as quality settings
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SaveVideoModeAndQuality();

	// Revert to original video settings
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool RevertVideoMode();

private:
	// Try to get the GameUserSettings object from the engine
	static UGameUserSettings* GetGameUserSettings();
};
