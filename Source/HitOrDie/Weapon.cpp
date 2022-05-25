#include "Weapon.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Hitter.h"
#include "Bullet.h"

const FName AWeapon::GripSocketName = FName(TEXT("GripPoint"));
const FName AWeapon::MuzzleSocketName = FName(TEXT("Muzzle"));

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->CastShadow = false;
}

void AWeapon::Fire()
{
	if (!Hitter)
	{
		return;
	}

	FTransform SpawnLocation = Mesh->GetSocketTransform(MuzzleSocketName);
	
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Instigator = Hitter;
	SpawnParameters.Owner = Hitter;
	
	GetWorld()->SpawnActor(BulletType, &SpawnLocation, SpawnParameters);
}

void AWeapon::AttachTo(AHitter* AttachedHitter)
{
	Hitter = AttachedHitter;
	if (Hitter)
	{
		AttachToComponent(Hitter->Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), GripSocketName);
	}
}