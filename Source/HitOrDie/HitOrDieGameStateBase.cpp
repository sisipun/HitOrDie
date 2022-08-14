#include "HitOrDieGameStateBase.h"

#include "GameFramework/PlayerState.h"

#include "EngineUtils.h"

AHitOrDieGameStateBase::AHitOrDieGameStateBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHitOrDieGameStateBase::BeginPlay()
{
	SoundEmitter = *TActorIterator<ASoundEmitter>(GetWorld());
}

void AHitOrDieGameStateBase::Auth_OnKilled(UPlayer* Hitter, UPlayer* Hitted)
{
	FString HitterName = Hitter->PlayerController->PlayerState->GetPlayerName();
	if (PlayersScore.Contains(HitterName))
	{
		PlayersScore.Add(HitterName, PlayersScore[HitterName] + 1);
	}
	else
	{
		PlayersScore.Add(HitterName, 1);
	}

	UE_LOG(LogTemp, Warning, TEXT("Score %s - %d"), *HitterName, PlayersScore[HitterName]);
}

bool AHitOrDieGameStateBase::PerformAction(EActionType Action)
{
	check(SoundEmitter);

	return SoundEmitter->PerformAction(Action);
}

TArray<FTiming> AHitOrDieGameStateBase::GetPossibleActions(float PeriodBefore, float PeriodAfter) const
{
	check(SoundEmitter);

	return SoundEmitter->GetPossibleActions(PeriodBefore, PeriodAfter);
}

float AHitOrDieGameStateBase::GetPlaybackValue() const
{
	check(SoundEmitter);

	return SoundEmitter->GetPlaybackValue();
}