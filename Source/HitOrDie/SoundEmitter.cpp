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

bool ASoundEmitter::GetPossibleAction() const
{
	return CanFire;
}

void ASoundEmitter::OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	float PlaybackValue = Audio->Sound->Duration * PlaybackPercent;
	UE_LOG(LogTemp, Warning, TEXT("Percent: %f, Current Position: %f"), PlaybackPercent, PlaybackValue);

	CanFire = false;
	for (TPair<float, bool> ActionTiming : SoundProperties.ActionTimings)
	{
		if (ActionTiming.Key < PlaybackValue && PlaybackValue < ActionTiming.Key + SoundProperties.ActionLenght)
		{
			CanFire = ActionTiming.Value;
		}
	}
}