#include "ExplosiveBullet.h"

#include "Components/SphereComponent.h"

#include "Hitter.h"
#include "HitterController.h"

AExplosiveBullet::AExplosiveBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	ExplosionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollider"));
	ExplosionCollider->SetGenerateOverlapEvents(true);
	ExplosionCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ExplosionCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ExplosionCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	ExplosionCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ExplosionCollider->SetupAttachment(RootComponent);
}

void AExplosiveBullet::Auth_OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(HasAuthority());
	AHitter* Hitter = Cast<AHitter>(GetOwner());
	AHitter* Hitted = Cast<AHitter>(OtherActor);
	if (!Hitter || !Hitted || Hitter->GetNetOwningPlayer() == Hitted->GetNetOwningPlayer())
	{
		return;
	}

	Auth_Explode();
}

void AExplosiveBullet::Auth_OnMiss(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	check(HasAuthority());
	Auth_Explode();
}

void AExplosiveBullet::Auth_Explode()
{
	check(HasAuthority());

	AHitter* Hitter = Cast<AHitter>(GetOwner());
	TSet<AActor*> OverlappingActors;
	ExplosionCollider->GetOverlappingActors(OverlappingActors);
	
	for (AActor*& Actor : OverlappingActors)
	{
		AHitter* Hitted = Cast<AHitter>(Actor);
		TObjectPtr<AHitterController> HitterController = Cast<AHitterController>(Hitter->GetNetOwningPlayer()->PlayerController);
		Hitted->Auth_Hit(HitterController, Power);
	}

	Destroy();
}