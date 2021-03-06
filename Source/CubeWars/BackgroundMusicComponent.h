// This source code file is protected by the MIT License (MIT)

#pragma once

#include "Components/SceneComponent.h"
#include "BackgroundMusicComponent.generated.h"


USTRUCT(BlueprintType)
struct FMusicTitle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	USoundBase* MusicSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	FString Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	FString Artist;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSongStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSongEnded);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUBEWARS_API UBackgroundMusicComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBackgroundMusicComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	bool bShuffleMusic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	bool bLoopPlaylist;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	bool bAutoplay;

	UFUNCTION()
	void SongEnded();

	UPROPERTY(BlueprintAssignable, Category = Music, meta = (DisplayName = "On Song Started"))
	FSongStarted SongStartedDelegate;

	UPROPERTY(BlueprintAssignable, Category = Music, meta = (DisplayName = "On Song Ended"))
	FSongEnded SongEndedDelegate;

	UFUNCTION(BlueprintCallable, Category = "Music")
	void NextSong();

	UFUNCTION(BlueprintCallable, Category = "Music")
	void Play();

	UFUNCTION(BlueprintCallable, Category = "Music")
	void Stop();

	UFUNCTION(BlueprintCallable, Category = "Music")
	FMusicTitle& GetCurrentMusicTitle();

private:
	void SetUpPlaylist();

	UPROPERTY(EditDefaultsOnly, Category = "Music")
	TArray<FMusicTitle> MusicTitles;

	TArray<int32> MusicPlaylist;
	int32 CurrentPlaylistIndex;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AttachedSound;
};
