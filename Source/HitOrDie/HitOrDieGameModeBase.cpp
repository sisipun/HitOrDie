#include "HitOrDieGameModeBase.h"

#include "GameFramework/GameState.h"
#include "EngineUtils.h"

#include "SoundEmitter.h"

AHitOrDieGameModeBase::AHitOrDieGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHitOrDieGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SoundEmitter = *TActorIterator<ASoundEmitter>(GetWorld());
}

void AHitOrDieGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	check(SoundEmitter);

	UE_LOG(LogTemp, Warning, TEXT("LOGIN"));

	if (GameState->PlayerArray.Num() == MinPlayersCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("START"));
		SoundEmitter->Auth_Play();
	}
}