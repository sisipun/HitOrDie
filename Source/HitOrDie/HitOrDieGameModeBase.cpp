#include "HitOrDieGameModeBase.h"

#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"

#include "HitterController.h"

AHitOrDieGameModeBase::AHitOrDieGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHitOrDieGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	SoundEmitter = *TActorIterator<ASoundEmitter>(GetWorld());
}

void AHitOrDieGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("%d - %d"), GameState->PlayerArray.Num(), MaxPlayersCount)
	if (GameState->PlayerArray.Num() >= MaxPlayersCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Too many players"))
		ErrorMessage = TEXT("Too many players");
	}
}

void AHitOrDieGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	check(SoundEmitter);

	if (GameState->PlayerArray.Num() == MinPlayersCount)
	{
		SoundEmitter->Auth_Play();
	}
}

bool AHitOrDieGameModeBase::Auth_PerformAction(AHitterController* Hitter, EActionType Action)
{
	check(HasAuthority());
	check(SoundEmitter);

	return SoundEmitter->Auth_PerformAction(Hitter, Action);
}

void AHitOrDieGameModeBase::Auth_OnKilled(TObjectPtr<AHitterController> Hitter, UPlayer* Hitted)
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