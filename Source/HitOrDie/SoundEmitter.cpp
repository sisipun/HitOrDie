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
	Audio->Sound = SoundProperties.Sound;
	Audio->OnAudioPlaybackPercent.AddDynamic(this, &ASoundEmitter::OnAudioPlaybackPercent);
	Audio->Play();
}

ActionType ASoundEmitter::GetPossibleAction() const
{
	return CurrentAction;
}

void ASoundEmitter::OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	float PlaybackValue = Audio->Sound->Duration * PlaybackPercent;
	UE_LOG(LogTemp, Warning, TEXT("Percent: %f, Current Position: %f"), PlaybackPercent, PlaybackValue);

	CurrentAction = ActionType::NONE;
	for (TPair<float, ActionType> ActionTiming : SoundProperties.ActionTimings)
	{
		if (ActionTiming.Key < PlaybackValue && PlaybackValue < ActionTiming.Key + SoundProperties.ActionLenght)
		{
			CurrentAction = ActionTiming.Value;
		}
	}
}