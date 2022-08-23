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