#include "HitOrDieGameStateBase.h"

#include "EngineUtils.h"
#include "SoundEmitter.h"

void AHitOrDieGameStateBase::BeginPlay()
{
	SoundEmitter = *TActorIterator<ASoundEmitter>(GetWorld());
}

EActionType AHitOrDieGameStateBase::GetPossibleAction() const
{
	check(SoundEmitter);

	return SoundEmitter->GetPossibleAction();
}