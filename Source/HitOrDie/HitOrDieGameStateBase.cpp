#include "HitOrDieGameStateBase.h"

#include "EngineUtils.h"
#include "SoundEmitter.h"

void AHitOrDieGameStateBase::BeginPlay()
{
	SoundEmitter = *TActorIterator<ASoundEmitter>(GetWorld());
}

bool AHitOrDieGameStateBase::GetPossibleAction() const
{
	return SoundEmitter && SoundEmitter->GetPossibleAction();
}