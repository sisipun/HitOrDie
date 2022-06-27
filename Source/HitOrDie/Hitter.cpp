#include "Hitter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Weapon.h"
#include "Bullet.h"

AHitter::AHitter()
{
	PrimaryActorTick.bCanEverTick = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetCapsuleComponent());

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
}

void AHitter::BeginPlay()
{
	Super::BeginPlay();
	
	FTransform SpawnTransform = Mesh1P->GetSocketTransform(AWeapon::GripSocketName);

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	CurrentWeapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponType, &SpawnTransform, spawnParameters));
	if (CurrentWeapon)
	{
		CurrentWeapon->AttachTo(this);
	}
}

void AHitter::Fire()
{
	if (CurrentWeapon)
	{
		FTransform SpawnLocation = CurrentWeapon->Mesh->GetSocketTransform(AWeapon::MuzzleSocketName);

		Server_SpawnBullet(CurrentWeapon->BulletType, SpawnLocation);
	}
}

void AHitter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}
}

void AHitter::Server_SpawnBullet_Implementation(TSubclassOf<ABullet> BulletType, FTransform SpawnLocation)
{
	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	GetWorld()->SpawnActor(BulletType, &SpawnLocation, spawnParameters);
}