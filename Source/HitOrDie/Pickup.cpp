#include "Pickup.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Hitter.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Mesh->SetupAttachment(RootComponent);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &APickup::Auth_OnOverlap);
	}
}


void APickup::Auth_OnOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(HasAuthority());
	AHitter* Overlaped = Cast<AHitter>(OtherActor);

	if (Overlaped)
	{
		Auth_PickedUp(Overlaped);
		Destroy();
	}
}