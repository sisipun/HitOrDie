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

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = Bullet)
	TSubclassOf<ABullet> BulletType;

public:	
	AWeapon();

	void AttachTo(AHitter* AttachedHitter);

public:
	static const FName GripSocketName;
	static const FName MuzzleSocketName;

private:
	AHitter* Hitter;

};
