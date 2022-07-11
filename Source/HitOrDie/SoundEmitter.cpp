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
		ActionTimings.KeySort([](const float& FirstKey, const float& SecondKey) {
			return FirstKey < SecondKey;
		});

		Audio->Sound = SoundProperties->Sound;
		Audio->OnAudioPlaybackPercent.AddDynamic(this, &ASoundEmitter::OnAudioPlaybackPercent);
		Audio->Play();
	}
}

EActionType ASoundEmitter::GetPossibleAction() const
{
	EActionType CurrentAction = EActionType::NONE;
	for (const TPair<float, EActionType>& ActionTiming : ActionTimings)
	{
		if (ActionTiming.Key < PlaybackValue && PlaybackValue < ActionTiming.Key + ActionLenght)
		{
			CurrentAction = ActionTiming.Value;
			break;
		}
	}
	return CurrentAction;
}

void ASoundEmitter::OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	PlaybackValue = Audio->Sound->Duration * PlaybackPercent;
}