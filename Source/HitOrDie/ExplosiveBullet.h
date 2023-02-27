#pragma once

#include "CoreMinimal.h"

#include "Bullet.h"

#include "ExplosiveBullet.generated.h"

UCLASS()
class HITORDIE_API AExplosiveBullet : public ABullet
{
	GENERATED_BODY()
	
protected:
	void Auth_OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void Auth_OnMiss(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USphereComponent> ExplosionCollider;

public:
	AExplosiveBullet();

private:
	void Auth_Explode();
};
