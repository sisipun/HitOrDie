#include "SoundEmitter.h"

#include "Components/AudioComponent.h"
#include "GameFramework/PlayerState.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"

#include "HitterController.h"

ASoundEmitter::ASoundEmitter()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetIsReplicated(true);
	Audio->bAutoActivate = false;
}

void ASoundEmitter::BeginPlay()
{
	Super::BeginPlay();
	PlaybackValue = 0.0f;

	TObjectPtr<FSoundProperties> SoundProperties = SoundPropertiesDataTable->FindRow<FSoundProperties>(CurrentSound, TEXT("Searching for sound properties"));
	if (SoundProperties)
	{
		if (HasAuthority())
		{
			ActionTimings = SoundProperties->ActionTimings;
			ActionTimings.Sort([](const FTiming& FirstKey, const FTiming& SecondKey) { return FirstKey.StartSecond < SecondKey.StartSecond; });

			Audio->OnAudioPlaybackPercent.AddDynamic(this, &ASoundEmitter::Auth_OnAudioPlaybackPercent);

			FTimerHandle CountdownTimer;
			GetWorldTimerManager().SetTimer(CountdownTimer, this, &ASoundEmitter::Auth_OnCountdownFinished, 3.0f, false);
		}

		Audio->Sound = SoundProperties->Sound;
	}
}

void ASoundEmitter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoundEmitter, ActionTimings);
	DOREPLIFETIME(ASoundEmitter, PlaybackValue);
	DOREPLIFETIME(ASoundEmitter, HitterActionIndices);
}

void ASoundEmitter::Multicast_StartSound_Implementation()
{
	Audio->Play();
}

void ASoundEmitter::Auth_OnCountdownFinished()
{
	check(HasAuthority());

	Multicast_StartSound();
}

void ASoundEmitter::Auth_OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	check(HasAuthority());

	PlaybackValue = Audio->Sound->Duration * PlaybackPercent;
}

void ASoundEmitter::OnRep_HitterActionIndices()
{
	for (const FHitterActionIndex& HitterActionIndex : HitterActionIndices)
	{
		if (HitterToActionIndex.Contains(HitterActionIndex.Name))
		{
			HitterToActionIndex[HitterActionIndex.Name] = HitterActionIndex.Index;
		}
		else
		{
			HitterToActionIndex.Add(HitterActionIndex.Name, HitterActionIndex.Index);
		}
	}
}

bool ASoundEmitter::Auth_PerformAction(TObjectPtr<AHitterController> Hitter, EActionType Action)
{
	check(HasAuthority());

	FString HitterName = Hitter->PlayerState->GetPlayerName();
	if (!HitterToActionIndex.Contains(HitterName))
	{
		HitterToActionIndex.Add(HitterName, 0);
	}

	int& HitterActionIndex = HitterToActionIndex[HitterName];
	while (ActionTimings.Num() < HitterActionIndex && ActionTimings[HitterActionIndex].EndSecond < PlaybackValue)
	{
		HitterActionIndex++;
		SyncActionIndex(HitterName, HitterActionIndex);
	}

	if (ActionTimings.Num() > HitterActionIndex && ActionTimings[HitterActionIndex].StartSecond < PlaybackValue && PlaybackValue < ActionTimings[HitterActionIndex].EndSecond)
	{
		HitterActionIndex++;
		SyncActionIndex(HitterName, HitterActionIndex);
		return true;
	}
	else
	{
		return false;
	}
}

TArray<FTiming> ASoundEmitter::GetPossibleActions(TObjectPtr<AHitterController> Hitter, float PeriodBefore, float PeriodAfter) const
{
	FString HitterName = Hitter->PlayerState->GetPlayerName();
	int HitterActionIndex = HitterToActionIndex.Contains(HitterName) ? HitterToActionIndex[HitterName] : 0;

	TArray<FTiming> Actions;
	for (int i = HitterActionIndex; i < ActionTimings.Num(); i++)
	{
		const FTiming& Timing = ActionTimings[i];
		if (Timing.StartSecond - PeriodBefore > PlaybackValue)
		{
			break;
		}
		else if (PlaybackValue <= Timing.EndSecond + PeriodAfter)
		{
			Actions.Add(Timing);
		}
	}
	return Actions;
}

float ASoundEmitter::GetPlaybackValue() const
{
	return PlaybackValue;
}

void ASoundEmitter::SyncActionIndex(FString Name, int Index)
{
	for (FHitterActionIndex& HitterActionIndex : HitterActionIndices)
	{
		if (HitterActionIndex.Name == Name)
		{
			HitterActionIndex.Index = Index;
			break;
		}
	}

	HitterActionIndices.Add({ Name, Index });
}