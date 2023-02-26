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

protected:
	UFUNCTION()
	void Auth_OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Auth_OnMiss(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = UProjectile)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float Power;

public:	
	ABullet();

protected:
	virtual void BeginPlay() override;
};
