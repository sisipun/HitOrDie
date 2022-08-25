#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Hitter.h"
#include "HitterController.h"
#include "Weapon.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetGenerateOverlapEvents(true);
	Collider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	InitialLifeSpan = 3.0f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &ABullet::Auth_OnHit);
		Collider->OnComponentHit.AddDynamic(this, &ABullet::Auth_OnMiss);
	}
}

void ABullet::Auth_OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(HasAuthority());
	AHitter* Hitter = Cast<AHitter>(GetOwner());
	AHitter* Hitted = Cast<AHitter>(OtherActor);

	if (Hitter && Hitted && Hitter->GetNetOwningPlayer() != Hitted->GetNetOwningPlayer())
	{
		TObjectPtr<AHitterController> HitterController = Cast<AHitterController>(Hitter->GetNetOwningPlayer()->PlayerController);
		Hitted->Auth_Hit(HitterController, Power);
		Destroy();
	}
}

void ABullet::Auth_OnMiss(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	check(HasAuthority());
	Destroy();
}