#include "HitOrDieGameStateBase.h"

#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"

#include "HitterController.h"

AHitOrDieGameStateBase::AHitOrDieGameStateBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHitOrDieGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	SoundEmitter = *TActorIterator<ASoundEmitter>(GetWorld());
}

bool AHitOrDieGameStateBase::Auth_PerformAction(AHitterController* Hitter, EActionType Action)
{
	check(HasAuthority());
	check(SoundEmitter);

	return SoundEmitter->Auth_PerformAction(Hitter, Action);
}

TArray<FTiming> AHitOrDieGameStateBase::GetPossibleActions(AHitterController* Hitter, float PeriodBefore, float PeriodAfter) const
{
	check(SoundEmitter);

	return SoundEmitter->GetPossibleActions(Hitter, PeriodBefore, PeriodAfter);
}

float AHitOrDieGameStateBase::GetPlaybackValue() const
{
	check(SoundEmitter);

	return SoundEmitter->GetPlaybackValue();
}

int AHitOrDieGameStateBase::GetCountdownValue() const
{
	check(SoundEmitter);

	return SoundEmitter->GetCountdownValue();
}

void AHitOrDieGameStateBase::Auth_OnKilled(TObjectPtr<AHitterController> Hitter, UPlayer* Hitted)
{
	check(HasAuthority());

	FString HitterName = Hitter->PlayerState->GetPlayerName();
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