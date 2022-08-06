#include "SoundEmitter.h"

#include "Components/AudioComponent.h"
#include "Math/UnrealMathUtility.h"

ASoundEmitter::ASoundEmitter()
{
	PrimaryActorTick.bCanEverTick = false;

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->bAutoActivate = false;
}

void ASoundEmitter::BeginPlay()
{
	Super::BeginPlay();
	PlaybackValue = 0.0f;

	TObjectPtr<FSoundProperties> SoundProperties = SoundPropertiesDataTable->FindRow<FSoundProperties>(CurrentSound, TEXT("Searching for sound properties"));
	if (SoundProperties)
	{
		ActionLenght = SoundProperties->ActionLenght;
		ActionTimings = SoundProperties->ActionTimings;
		ActionTimings.Sort([](const FTiming& FirstKey, const FTiming& SecondKey) {
			return FirstKey.StartSecond < SecondKey.StartSecond;
		});

		Audio->Sound = SoundProperties->Sound;
		Audio->OnAudioPlaybackPercent.AddDynamic(this, &ASoundEmitter::OnAudioPlaybackPercent);
		Audio->Play();
	}
}

EActionType ASoundEmitter::GetPossibleAction() const
{
	EActionType CurrentAction = EActionType::NONE;
	for (const FTiming& ActionTiming : ActionTimings)
	{
		if (ActionTiming.StartSecond < PlaybackValue && PlaybackValue < ActionTiming.StartSecond + ActionLenght)
		{
			CurrentAction = ActionTiming.Action;
			break;
		}
	}
	return CurrentAction;
}

void ASoundEmitter::OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	PlaybackValue = Audio->Sound->Duration * PlaybackPercent;
}