// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "BackgroundMusicComponent.h"


// Sets default values for this component's properties
UBackgroundMusicComponent::UBackgroundMusicComponent() :
	bShuffleMusic(true),
	bLoopPlaylist(true),
	bAutoplay(true),
	AttachedSound(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBackgroundMusicComponent::BeginPlay()
{
	Super::BeginPlay();

	MusicPlaylist.AddZeroed(MusicTitles.Num());

	for(int32 i = 0; i < MusicPlaylist.Num(); i++)
	{
		MusicPlaylist[i] = i;
	}

	SetUpPlaylist();
	
	if(bAutoplay)
	{
		Play();
	}
}

void UBackgroundMusicComponent::BeginDestroy()
{
	SongEndedDelegate.Clear();
	SongStartedDelegate.Clear();

	Super::BeginDestroy();
}

// Called every frame
void UBackgroundMusicComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UBackgroundMusicComponent::SongEnded()
{
	UE_LOG(LogTemp, Warning, TEXT("Song Ended!"));
	SongEndedDelegate.Broadcast();

	if(bAutoplay)
	{
		NextSong();
	}
}

void UBackgroundMusicComponent::SetUpPlaylist()
{
	if(bShuffleMusic)
	{
		FRandomStream RandStream;
		RandStream.GenerateNewSeed();

		TArray<int32> AvailableNumbers;

		AvailableNumbers.AddZeroed(MusicTitles.Num());

		for(int32 i = 0; i < AvailableNumbers.Num(); i++)
		{
			AvailableNumbers[i] = i;
		}

		for(int32 i = 0; i < MusicPlaylist.Num(); i++)
		{
			int randomIndex = RandStream.RandRange(0, AvailableNumbers.Num() - 1);

			int songIndex = AvailableNumbers[randomIndex];

			AvailableNumbers.RemoveAt(randomIndex);
			MusicPlaylist[i] = songIndex;
		}
	}

	CurrentPlaylistIndex = 0;
}

void UBackgroundMusicComponent::NextSong()
{
	Stop();

	if(CurrentPlaylistIndex + 1 >= MusicPlaylist.Num() && !bLoopPlaylist)
	{
		return;
	}

	CurrentPlaylistIndex++;

	if(CurrentPlaylistIndex >= MusicPlaylist.Num())
	{
		SetUpPlaylist();
	}

	Play();
}

void UBackgroundMusicComponent::Play()
{
	if(AttachedSound == nullptr)
	{
		FAttenuationSettings attenuationSettings;
		attenuationSettings.bAttenuate = false;
		attenuationSettings.bSpatialize = false;

		AttachedSound = UGameplayStatics::SpawnSoundAttached(MusicTitles[MusicPlaylist[CurrentPlaylistIndex]].MusicSound, this, TEXT("MusicSoundComponent"));
		AttachedSound->AdjustAttenuation(attenuationSettings);
		AttachedSound->OnAudioFinished.AddDynamic(this, &UBackgroundMusicComponent::SongEnded);
		AttachedSound->bAutoDestroy = false;

		SongStartedDelegate.Broadcast();
	}
	else
	{
		Stop();

		AttachedSound->Sound = MusicTitles[MusicPlaylist[CurrentPlaylistIndex]].MusicSound;
		AttachedSound->Play();

		SongStartedDelegate.Broadcast();
	}

	UE_LOG(LogTemp, Warning, TEXT("Song started!"));
}

void UBackgroundMusicComponent::Stop()
{
	if(AttachedSound != nullptr && AttachedSound->IsValidLowLevel() && AttachedSound->IsPlaying())
	{
		AttachedSound->Stop();
	}
}

FMusicTitle& UBackgroundMusicComponent::GetCurrentMusicTitle()
{
	return MusicTitles[MusicPlaylist[CurrentPlaylistIndex]];
}
