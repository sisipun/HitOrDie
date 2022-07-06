#include "HitOrDieGameStateBase.h"

#include "EngineUtils.h"
#include "SoundEmitter.h"

void AHitOrDieGameStateBase::BeginPlay()
{
	SoundEmitter = *TActorIterator<ASoundEmitter>(GetWorld());
}

ActionType AHitOrDieGameStateBase::GetPossibleAction() const
{
	check(SoundEmitter);

	return SoundEmitter->GetPossibleAction();
}