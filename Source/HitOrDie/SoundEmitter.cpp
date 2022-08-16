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
			ActionTimings.Sort([](const FTiming& FirstKey, const FTiming& SecondKey)
				{
					return FirstKey.StartSecond < SecondKey.StartSecond;
				}
			);

			Audio->OnAudioPlaybackPercent.AddDynamic(this, &ASoundEmitter::Auth_OnAudioPlaybackPercent);
		}

		Audio->Sound = SoundProperties->Sound;
		Audio->Play();
	}
}

void ASoundEmitter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoundEmitter, ActionTimings);
	DOREPLIFETIME(ASoundEmitter, PlaybackValue);
}

void ASoundEmitter::Auth_OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	check(HasAuthority());

	PlaybackValue = Audio->Sound->Duration * PlaybackPercent;
}

bool ASoundEmitter::Auth_PerformAction(UPlayer* Hitter, EActionType Action)
{
	check(HasAuthority());

	FString HitterName = Hitter->PlayerController->PlayerState->GetPlayerName();
	if (!HitterActionIndices.Contains(HitterName))
	{
		HitterActionIndices.Add(HitterName, 0);
	}

	int& HitterActionIndex = HitterActionIndices[HitterName];
	while (ActionTimings.Num() < HitterActionIndex && ActionTimings[HitterActionIndex].EndSecond < PlaybackValue)
	{
		HitterActionIndex++;
	}

	if (ActionTimings.Num() >= HitterActionIndex && ActionTimings[HitterActionIndex].StartSecond < PlaybackValue && PlaybackValue < ActionTimings[HitterActionIndex].EndSecond)
	{
		HitterActionIndex++;
		return true;
	}
	else
	{
		return false;
	}
}

TArray<FTiming> ASoundEmitter::GetPossibleActions(AHitterController* Hitter, float PeriodBefore, float PeriodAfter) const
{
	FString HitterName = Hitter->PlayerState->GetPlayerName();
	int HitterActionIndex = HitterActionIndices.Contains(HitterName) ? HitterActionIndices[HitterName] : 0;
	
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
	UE_LOG(LogTemp, Warning, TEXT("PV %f"), PlaybackValue);
	UE_LOG(LogTemp, Warning, TEXT("ACTIONS %d"), Actions.Num());
	return Actions;
}

float ASoundEmitter::GetPlaybackValue() const
{
	return PlaybackValue;
}