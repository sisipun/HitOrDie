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

TArray<FTiming> AHitOrDieGameStateBase::GetPossibleActions(float HalfPeriod) const
{
	check(SoundEmitter);

	return SoundEmitter->GetPossibleActions(HalfPeriod);
}

bool AHitOrDieGameStateBase::PerformAction(EActionType Action)
{
	check(SoundEmitter);

	return SoundEmitter->PerformAction(Action);
}