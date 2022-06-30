#include "SoundEmitter.h"

#include "Components/AudioComponent.h"

ASoundEmitter::ASoundEmitter()
{
	PrimaryActorTick.bCanEverTick = false;

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}

void ASoundEmitter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Audio->Play();
	}
}
