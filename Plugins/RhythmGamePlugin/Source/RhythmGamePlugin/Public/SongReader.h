// Copyright 2019 Phoenix Market

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/MovieSceneTracks/Public/Sections/MovieSceneAudioSection.h"
#include "SongReader.generated.h"

USTRUCT(BlueprintType)
struct FSongData
{
	GENERATED_BODY()

	FSongData()
	{
		autoCalculateBPM = true;
		manualBPM = 100;
	}

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SongData")
	bool autoCalculateBPM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SongData")
	float manualBPM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SongData")
	ULevelSequence* songSequence;
};

USTRUCT(BlueprintType)
struct FContinuousBeat
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "ContinuousBeat")
	float startTime;

	UPROPERTY(BlueprintReadWrite, Category = "ContinuousBeat")
	float endTime;
};

USTRUCT(BlueprintType)
struct FLaneData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "LaneData")
	TArray<float> listOfSingleBeats;

	UPROPERTY(BlueprintReadWrite, Category = "LaneData")
	TArray<FContinuousBeat> listOfContinuousBeats;
};

UCLASS()
class RHYTHMGAMEPLUGIN_API ASongReader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASongReader();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SongReader")
	TMap<FString, FSongData> listOfSongs;

	UPROPERTY(BlueprintReadOnly, Category = "SongReader")
	float autoBPM;

	UPROPERTY(BlueprintReadOnly, Category = "SongReader")
	USoundBase* song;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "SongReader", meta = (Keywords = "SongReader"))
	TArray<FLaneData> GetLaneData(FSongData songData);

	UFUNCTION(BlueprintCallable, Category = "SongReader", meta = (Keywords = "SongReader"))
	USoundBase* GetSong(FSongData songData);

	UFUNCTION(BlueprintCallable, Category = "SongReader", meta = (Keywords = "SongReader"))
	void SetSongVolume(float volume);

	UFUNCTION(BlueprintCallable, Category = "SongReader", meta = (Keywords = "SongReader"))
	void SetSongPitch(float pitch);

private:
	void SetSong(UMovieSceneAudioSection* audioSection);
};
