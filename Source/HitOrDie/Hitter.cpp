#include "Hitter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

#include "Weapon.h"
#include "Bullet.h"
#include "HitOrDieGameModeBase.h"
#include "SoundEmitter.h"
#include "HitterController.h"

const FName AHitter::GripSocketName = FName(TEXT("GripPoint"));

AHitter::AHitter()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetCapsuleComponent());

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh3P"));
	Mesh3P->SetupAttachment(Camera);
	Mesh3P->SetOwnerNoSee(true);
	Mesh3P->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void AHitter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHitter, Health);
	DOREPLIFETIME(AHitter, bDead);
	DOREPLIFETIME(AHitter, bActionCooldown);
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
	bDead = false;
	bActionCooldown = false;
	SpawnWeapon();
}

void AHitter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}
}

void AHitter::Server_Fire_Implementation()
{
	if (!CurrentWeapon || IsDead())
	{
		return;
	}

	FTransform SpawnLocation = CurrentWeapon->GetMuzzleTransform();
	TSubclassOf<ABullet> BulletType = CurrentWeapon->GetBulletType();

	AHitOrDieGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AHitOrDieGameModeBase>();
	check(GameMode);

	TObjectPtr<AHitterController> HitterController = Cast<AHitterController>(GetNetOwningPlayer()->PlayerController);
	if (!bActionCooldown && GameMode->Auth_PerformAction(HitterController, EActionType::FIRE))
	{
		Auth_SpawnBullet(BulletType, SpawnLocation);
	}
	else
	{
		bActionCooldown = true;
		GetWorldTimerManager().SetTimer(ActionCooldownTimer, this, &AHitter::Auth_OnActionCooldownFinished, 3.0f, false);
	}
}

void AHitter::OnRep_bDead()
{
	if (IsDead())
	{
		Mesh1P->SetSimulatePhysics(true);
		Mesh3P->SetSimulatePhysics(true);
	}
}

void AHitter::Jump()
{
	if (!IsDead())
	{
		Super::Jump();
	}
}

void AHitter::StopJumping()
{
	if (!IsDead())
	{
		Super::StopJumping();
	}
}

void AHitter::MoveX(float Scale)
{
	if (!IsDead() && Scale != 0)
	{
		AddMovementInput(GetActorRightVector(), Scale);
	}
}

void AHitter::MoveY(float Scale)
{
	if (!IsDead() && Scale != 0)
	{
		AddMovementInput(GetActorForwardVector(), Scale);
	}
}

void AHitter::Fire()
{
	Server_Fire();
}

void AHitter::Auth_Hit(TObjectPtr<AHitterController> Hitter, float Value)
{
	check(HasAuthority());
	if (IsDead())
	{
		return;
	}

	Health -= Value;
	if (Health <= 0.0f)
	{
		Health = 0.0f;
		bDead = true;
		OnRep_bDead();

		FTimerHandle DeadTimer;
		GetWorldTimerManager().SetTimer(DeadTimer, this, &AHitter::Auth_OnDead, 3.0f, false);

		AHitOrDieGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AHitOrDieGameModeBase>();
		check(GameMode);
		GameMode->Auth_OnKilled(Hitter, GetNetOwningPlayer());
	}
}

bool AHitter::IsDead() const
{
	return bDead;
}

TObjectPtr<USkeletalMeshComponent> AHitter::GetMesh() const
{
	return HasAuthority() || IsLocallyControlled() ? Mesh1P : Mesh3P;
}

void AHitter::Auth_OnActionCooldownFinished()
{
	check(HasAuthority());

	bActionCooldown = false;
}

void AHitter::Auth_OnDead()
{
	check(HasAuthority());

	OnDead.Broadcast();
}

void AHitter::Auth_SpawnBullet(TSubclassOf<ABullet> BulletType, FTransform SpawnLocation)
{
	check(HasAuthority());

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	GetWorld()->SpawnActor(BulletType, &SpawnLocation, spawnParameters);
}

void AHitter::SpawnWeapon()
{
	FTransform SpawnTransform = GetMesh()->GetSocketTransform(GripSocketName);

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	CurrentWeapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponType, &SpawnTransform, spawnParameters));
	if (CurrentWeapon)
	{
		CurrentWeapon->AttachTo(this);
	}
}