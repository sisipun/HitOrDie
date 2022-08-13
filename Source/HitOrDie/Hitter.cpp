#include "Hitter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

#include "Weapon.h"
#include "Bullet.h"
#include "HitOrDieGameStateBase.h"
#include "SoundEmitter.h"
#include "HitterController.h"

const FName AHitter::GripSocketName = FName(TEXT("GripPoint"));

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
	SpawnWeapon();
}

void AHitter::SpawnWeapon()
{
	FTransform SpawnTransform = IsLocallyControlled()
		? Mesh1P->GetSocketTransform(GripSocketName)
		: Mesh3P->GetSocketTransform(GripSocketName);

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

	if (CurrentWeapon && GameState->PerformAction(EActionType::FIRE))
	{
		FTransform SpawnLocation = CurrentWeapon->GetMuzzleTransform();

		Server_SpawnBullet(CurrentWeapon->GetBulletType(), SpawnLocation);
	}
}

void AHitter::Auth_Hit(UPlayer* Hitter, float Value)
{
	check(HasAuthority());

	Health -= Value;
	if (Health <= 0.0f)
	{
		Health = 0.0f;
		bDead = true;
		OnRep_bDead();

		FTimerHandle DeadTimer;
		GetWorldTimerManager().SetTimer(DeadTimer, Cast<AHitterController>(GetController()), &AHitterController::Auth_OnDead, 3.0f, false);

		AHitOrDieGameStateBase* GameState = Cast<AHitOrDieGameStateBase>(GetWorld()->GetGameState());
		check(GameState);
		GameState->Auth_OnKilled(Hitter, GetNetOwningPlayer());
	}

	UE_LOG(LogTemp, Warning, TEXT("Hit %f, Current Health %f"), Value, Health);
}

void AHitter::OnRep_Health()
{
	UE_LOG(LogTemp, Warning, TEXT("Hitted. Current Health %f"), Health);
}

void AHitter::OnRep_bDead()
{
	if (bDead)
	{
		Mesh1P->SetSimulatePhysics(true);
		Mesh3P->SetSimulatePhysics(true);
	}
}

void AHitter::Server_SpawnBullet_Implementation(TSubclassOf<ABullet> BulletType, FTransform SpawnLocation)
{
	check(HasAuthority());

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	GetWorld()->SpawnActor(BulletType, &SpawnLocation, spawnParameters);
}

TObjectPtr<USkeletalMeshComponent> AHitter::GetMesh() const
{
	return IsLocallyControlled() ? Mesh1P : Mesh3P;
}

bool AHitter::IsDead() const
{
	return bDead;
}

void AHitter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}
}