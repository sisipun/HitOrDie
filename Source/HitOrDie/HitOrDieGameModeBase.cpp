#include "HitOrDieGameModeBase.h"

AHitOrDieGameModeBase::AHitOrDieGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHitOrDieGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("LOGIN"));
}