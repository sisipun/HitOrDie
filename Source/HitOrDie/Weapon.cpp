#include "Weapon.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Hitter.h"
#include "Bullet.h"

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

	FTransform SpawnLocation = Mesh->GetSocketTransform(FName(TEXT("Muzzle")));
	//FRotator SpawnRotation = Cast<APlayerController>(Hitter->GetController())->PlayerCameraManager->GetCameraRotation();
	
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Instigator = Hitter;
	SpawnParameters.Owner = Hitter;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	GetWorld()->SpawnActor(BulletType, &SpawnLocation, SpawnParameters);
}

void AWeapon::Attach(AHitter* AttachedHitter)
{
	Hitter = AttachedHitter;
	if (Hitter)
	{
		AttachToComponent(Hitter->Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName(TEXT("GripPoint")));
	}
}