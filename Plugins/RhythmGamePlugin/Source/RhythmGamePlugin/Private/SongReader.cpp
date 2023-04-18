// Copyright 2019 Phoenix Market

#include "SongReader.h"
#include "Runtime/MovieSceneTracks/Public/Sections/MovieSceneEventTriggerSection.h"
#include "Runtime/MovieSceneTracks/Public/Sections/MovieSceneEventRepeaterSection.h"
#include "Runtime/MovieSceneTracks/Public/Tracks/MovieSceneEventTrack.h"
#include "Runtime/MovieSceneTracks/Public/Tracks/MovieSceneAudioTrack.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"

// Sets default values
ASongReader::ASongReader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASongReader::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASongReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<FLaneData> ASongReader::GetLaneData(FSongData songData)
{
	TArray<FLaneData> laneDataList;

	int beatCounter = 0;

	TRange<FFrameNumber> rangeNumber = songData.songSequence->GetMovieScene()->GetPlaybackRange();

	// Starting frame of the sequence
	int songSequenceStartFrame = rangeNumber.GetLowerBoundValue().Value;

	// Ending frame of the sequence
	int songSequenceEndFrame = rangeNumber.GetUpperBoundValue().Value;

	// Duration of the sequence in frames
	int songSequenceDuration = songSequenceEndFrame - songSequenceStartFrame;

	FVector2D inRange(songSequenceStartFrame, songSequenceEndFrame);
	FVector2D outRange(0, 1);

	TArray<UMovieSceneTrack*> allTracks = songData.songSequence->GetMovieScene()->GetMasterTracks();

	// Go through all the tracks
	for (int i = 0; i < allTracks.Num(); i++)
	{
		UMovieSceneAudioTrack* audioTrack = Cast<UMovieSceneAudioTrack>(allTracks[i]);

		// Find audio track and set the song
		if (audioTrack != nullptr)
		{
			TArray<UMovieSceneSection*> movieSceneSections = audioTrack->GetAudioSections();
			UMovieSceneAudioSection* audioSection = Cast<UMovieSceneAudioSection>(movieSceneSections[0]);
			SetSong(audioSection);
			continue;
		}

		UMovieSceneEventTrack* eventTrack = Cast<UMovieSceneEventTrack>(allTracks[i]);

		if (eventTrack == nullptr)
			continue;

		FLaneData laneData;

		TArray<UMovieSceneSection*> sections = eventTrack->GetAllSections();

		// Go through all the event sections
		for (int j = 0; j < sections.Num(); j++)
		{
			UMovieSceneEventTriggerSection* eventTriggerSection = Cast<UMovieSceneEventTriggerSection>(sections[j]);

			if (eventTriggerSection != nullptr)
			{
				FMovieSceneEventChannel movieSceneEventChannel = eventTriggerSection->EventChannel;
				TMovieSceneChannelData<FMovieSceneEvent> movieSceneEvent = movieSceneEventChannel.GetData();;

				TArrayView<FFrameNumber> frameNumberList = movieSceneEvent.GetTimes();

				// Go through all the event key frames
				for (int k = 0; k < frameNumberList.Num(); k++)
				{
					if (frameNumberList[k].Value < songSequenceStartFrame || frameNumberList[k].Value > songSequenceEndFrame)
						continue;
					
					float beatPosition = FMath::GetMappedRangeValueClamped(inRange, outRange, float(frameNumberList[k].Value));
					
					beatCounter++;
					laneData.listOfSingleBeats.Add(beatPosition);
				}
			}

			UMovieSceneEventRepeaterSection* eventRepeaterSection = Cast<UMovieSceneEventRepeaterSection>(sections[j]);

			if (eventRepeaterSection != nullptr)
			{
				FContinuousBeat continuousBeat;

				TRange<FFrameNumber> eventRange = eventRepeaterSection->GetRange();

				int continuousBeatStartTime = eventRange.GetLowerBoundValue().Value;
				int continuousBeatEndTime = eventRange.GetUpperBoundValue().Value;

				if (continuousBeatStartTime < songSequenceStartFrame || continuousBeatEndTime > songSequenceEndFrame)
					continue;

				continuousBeat.startTime = FMath::GetMappedRangeValueClamped(inRange, outRange, (float)continuousBeatStartTime);
				continuousBeat.endTime = FMath::GetMappedRangeValueClamped(inRange, outRange, (float)continuousBeatEndTime);
				
				beatCounter++;
				laneData.listOfContinuousBeats.Add(continuousBeat);
			}
		}

		laneDataList.Add(laneData);

		laneData.listOfSingleBeats.Empty();
		laneData.listOfContinuousBeats.Empty();
	}

	// Duration of the sequence in seconds
	float seqLengthSeconds = songSequenceDuration / 24000;

	autoBPM = (60 * beatCounter) / seqLengthSeconds;

	return laneDataList;
}

void ASongReader::SetSong(UMovieSceneAudioSection* audioSection)
{
	song = audioSection->GetSound();
}

USoundBase* ASongReader::GetSong(FSongData songData)
{
	USoundBase* previewSong = nullptr;

	TArray<UMovieSceneSection*> Sections = songData.songSequence->GetMovieScene()->GetAllSections();

	for (int i = 0; i < Sections.Num(); i++)
	{
		UMovieSceneAudioSection* audioSection = Cast<UMovieSceneAudioSection>(Sections[i]);

		if (audioSection != nullptr)
			previewSong = audioSection->GetSound();
	}

	return previewSong;
}

void ASongReader::SetSongVolume(float volume)
{
	USoundWave* soundWaveSong = Cast<USoundWave>(song);

	if (soundWaveSong != nullptr)
		soundWaveSong->Volume = volume;
	else
	{
		USoundCue* soundCueSong = Cast<USoundCue>(song);

		if (soundCueSong != nullptr)
			soundCueSong->VolumeMultiplier = volume;
	}
}

void ASongReader::SetSongPitch(float pitch)
{
	USoundWave* soundWaveSong = Cast<USoundWave>(song);

	if (soundWaveSong != nullptr)
		soundWaveSong->Pitch = pitch;
	else
	{
		USoundCue* soundCueSong = Cast<USoundCue>(song);

		if (soundCueSong != nullptr)
			soundCueSong->PitchMultiplier = pitch;
	}
}