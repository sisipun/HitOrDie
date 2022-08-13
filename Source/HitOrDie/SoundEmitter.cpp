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
		SoundProperties->ActionTimings.Sort([](const FTiming& FirstKey, const FTiming& SecondKey) {
			return FirstKey.StartSecond < SecondKey.StartSecond;
		});

		for (const FTiming& Timing : SoundProperties->ActionTimings)
		{
			ActionTimings.AddTail(Timing);
		}

		Audio->Sound = SoundProperties->Sound;
		Audio->OnAudioPlaybackPercent.AddDynamic(this, &ASoundEmitter::OnAudioPlaybackPercent);
		Audio->Play();
	}
}

TArray<FTiming> ASoundEmitter::GetPossibleActions(float HalfPeriod) const
{
	TArray<FTiming> Actions;
	for (const FTiming& Timing : ActionTimings)
	{
		if (Timing.StartSecond < PlaybackValue + HalfPeriod && PlaybackValue < Timing.EndSecond - HalfPeriod)
		{
			Actions.Add(Timing);
		}
	}
	return Actions;
}

bool ASoundEmitter::PerformAction(EActionType Action)
{
	TDoubleLinkedList<FTiming>::TDoubleLinkedListNode* Head = ActionTimings.GetHead();
	while (Head != nullptr && Head->GetValue().EndSecond < PlaybackValue)
	{
		ActionTimings.RemoveNode(Head);
		Head = ActionTimings.GetHead();
	}

	if (Head != nullptr && Head->GetValue().StartSecond < PlaybackValue && PlaybackValue < Head->GetValue().EndSecond)
	{
		ActionTimings.RemoveNode(Head);
		return true;
	}
	else
	{
		return false;
	}
}

void ASoundEmitter::OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	PlaybackValue = Audio->Sound->Duration * PlaybackPercent;
}