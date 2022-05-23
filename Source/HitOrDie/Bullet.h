#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class HITORDIE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = UProjectile)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

public:	
	ABullet();

};
