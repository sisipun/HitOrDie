#include "Hitter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Weapon.h"

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
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Instigator = this;
	SpawnParameters.Owner = this;
	
	CurrentWeapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponType, &SpawnTransform, SpawnParameters));
	if (CurrentWeapon)
	{
		CurrentWeapon->AttachTo(this);
	}
}

void AHitter::Fire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
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