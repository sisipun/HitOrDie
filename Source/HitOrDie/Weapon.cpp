#include "Weapon.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Hitter.h"
#include "Bullet.h"

const FName AWeapon::MuzzleSocketName = FName(TEXT("Muzzle"));

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = false;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetGenerateOverlapEvents(false);
	Collider->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Collider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetupAttachment(RootComponent);
	Mesh->CastShadow = false;
}

void AWeapon::AttachTo(AHitter* AttachedHitter)
{
	Hitter = AttachedHitter;
	if (Hitter)
	{
		AttachToComponent(
			Hitter->GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), 
			AHitter::GripSocketName
		);
	}
}

TSubclassOf<ABullet> AWeapon::GetBulletType() const
{
	return BulletType;
}

TSubclassOf<ABullet> AWeapon::GetGrenadeType() const
{
	return GrenadeType;
}

FTransform AWeapon::GetMuzzleTransform() const
{
	return Mesh->GetSocketTransform(AWeapon::MuzzleSocketName);
}