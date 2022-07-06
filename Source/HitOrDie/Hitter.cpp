#include "Hitter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

#include "Weapon.h"
#include "Bullet.h"
#include "HitOrDieGameStateBase.h"
#include "SoundEmitter.h"

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

	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh3P"));
	Mesh3P->SetupAttachment(Camera);
	Mesh3P->SetOwnerNoSee(true);
}

void AHitter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHitter, Health);
}

void AHitter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AHitter, MaxHealth))
	{
		Health = MaxHealth;
	}
}

void AHitter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	SpawnWeapon();
}

void AHitter::SpawnWeapon()
{
	FTransform SpawnTransform = IsLocallyControlled()
		? Mesh1P->GetSocketTransform(AWeapon::GripSocketName)
		: Mesh3P->GetSocketTransform(AWeapon::GripSocketName);

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
	AHitOrDieGameStateBase* GameState = Cast<AHitOrDieGameStateBase>(GetWorld()->GetGameState());
	check(GameState);

	if (CurrentWeapon && GameState->GetPossibleAction() == ActionType::FIRE)
	{
		FTransform SpawnLocation = CurrentWeapon->Mesh->GetSocketTransform(AWeapon::MuzzleSocketName);

		Server_SpawnBullet(CurrentWeapon->BulletType, SpawnLocation);
	}
}

void AHitter::Hit(float Value)
{
	Health -= Value;
	if (Health < 0.0f)
	{
		Health = 0.0f;
	}

	UE_LOG(LogTemp, Warning, TEXT("Hit %f, Current Health %f"), Value, Health);
}

void AHitter::OnRep_Health()
{
	UE_LOG(LogTemp, Warning, TEXT("Hitted. Current Health %f"), Health);
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