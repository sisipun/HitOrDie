#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Weapon.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class ABullet;
class AHitter;

UCLASS()
class HITORDIE_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = Bullet)
	TSubclassOf<ABullet> BulletType;

private:
	UPROPERTY(Transient)
	TObjectPtr<AHitter> Hitter;

public:	
	AWeapon();

	void AttachTo(AHitter* AttachedHitter);

	TSubclassOf<ABullet> GetBulletType() const;

	FTransform GetMuzzleTransform() const;

public:
	static const FName MuzzleSocketName;
};
