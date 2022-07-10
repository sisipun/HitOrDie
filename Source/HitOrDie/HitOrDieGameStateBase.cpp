#include "HitOrDieGameStateBase.h"

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

EActionType AHitOrDieGameStateBase::GetPossibleAction() const
{
	check(SoundEmitter);

	return SoundEmitter->GetPossibleAction();
}