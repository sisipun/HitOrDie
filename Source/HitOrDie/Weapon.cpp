#include "Weapon.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

#include "Bullet.h"
#include "Hitter.h"
#include "HitterController.h"

const FName AWeapon::MuzzleSocketName = FName(TEXT("Muzzle"));

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

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

	BulletCount = MaxBulletCount;
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, BulletCount);
}

void AWeapon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AWeapon, MaxBulletCount))
	{
		BulletCount = MaxBulletCount;
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	BulletCount = MaxBulletCount;
}

void AWeapon::Auth_Fire(const FVector& From, const FVector& Direction)
{
	check(HasAuthority());

	if (BulletCount > 0)
	{
		Auth_FireLineTrace(From, Direction);
		BulletCount--;
	}
	else
	{
		BulletCount = MaxBulletCount;
	}
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

FTransform AWeapon::GetMuzzleTransform() const
{
	return Mesh->GetSocketTransform(AWeapon::MuzzleSocketName);
}

void AWeapon::Auth_FireLineTrace(const FVector& From, const FVector& Direction)
{
	FVector Start = From;
	FVector End = From + (Direction * 10000);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Hitter);

	FHitResult Result;

	bool WasHitted = GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Pawn, CollisionParams, FCollisionResponseParams());
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0.1f, 10.0f);
	if (!WasHitted || !Result.GetActor())
	{
		return;
	}

	AHitter* Hitted = Cast<AHitter>(Result.GetActor());
	if (Hitter && Hitted && Hitter->GetNetOwningPlayer() != Hitted->GetNetOwningPlayer())
	{
		TObjectPtr<AHitterController> HitterController = Cast<AHitterController>(Hitter->GetNetOwningPlayer()->PlayerController);
		Hitted->Auth_Hit(HitterController, Power);
	}
}