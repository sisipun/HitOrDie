#include "HitOrDieGameStateBase.h"

#include "GameFramework/PlayerState.h"

#include "EngineUtils.h"
#include "SoundEmitter.h"

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

EActionType AHitOrDieGameStateBase::GetPossibleAction() const
{
	check(SoundEmitter);

	return SoundEmitter->GetPossibleAction();
}