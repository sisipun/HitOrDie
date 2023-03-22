#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Weapon.generated.h"

class ABullet;
class AHitter;
class USkeletalMeshComponent;
class USphereComponent;

UCLASS()
class HITORDIE_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = Ammunition)
	TSubclassOf<ABullet> BulletType;

	UPROPERTY(EditAnywhere, Category = Ammunition)
	TSubclassOf<ABullet> GrenadeType;

private:
	UPROPERTY(Transient)
	TObjectPtr<AHitter> Hitter;

public:	
	AWeapon();

	void AttachTo(AHitter* AttachedHitter);

	TSubclassOf<ABullet> GetBulletType() const;

	TSubclassOf<ABullet> GetGrenadeType() const;

	FTransform GetMuzzleTransform() const;

public:
	static const FName MuzzleSocketName;
};
