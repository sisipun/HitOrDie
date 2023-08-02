#include "Ability.h"

#include "Net/UnrealNetwork.h"

#include "Hitter.h"

AAbility::AAbility()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

void AAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAbility, bReloading);
}

void AAbility::BeginPlay()
{
	Super::BeginPlay();

	bReloading = false;
}

void AAbility::AttachTo(AHitter* AttachedHitter)
{
	Hitter = AttachedHitter;
}

void AAbility::Auth_TryUse()
{
	if (!bReloading)
	{
		bReloading = true;
		GetWorldTimerManager().SetTimer(AbilityReloadTimer, this, &AAbility::Auth_OnAbilityReloadFinished, ReloadDuration, false);
		Auth_Use();
	}
}

bool AAbility::CanUse() const
{
	return !bReloading;
}

void AAbility::Auth_OnAbilityReloadFinished()
{
	check(HasAuthority());
	
	bReloading = false;
}