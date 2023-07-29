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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float Power;

	UPROPERTY(EditAnywhere, Category = Ammunition)
	TSubclassOf<ABullet> BulletType;

	UPROPERTY(EditAnywhere, Category = Ammunition)
	TSubclassOf<ABullet> GrenadeType;

	UPROPERTY(EditAnywhere, Category = Ammunition)
	int32 BulletCount;


private:
	UPROPERTY(Transient)
	TObjectPtr<AHitter> Hitter;

public:	
	AWeapon();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void AttachTo(AHitter* AttachedHitter);

	void Auth_Fire(const FVector& From, const FVector& Direction);
	
	void Auth_Grenade();

	TSubclassOf<ABullet> GetBulletType() const;

	TSubclassOf<ABullet> GetGrenadeType() const;

	FTransform GetMuzzleTransform() const;

protected:
	virtual void BeginPlay() override;

private:
	void Auth_SpawnBullet(TSubclassOf<ABullet> Type, FTransform SpawnLocation);

	void Auth_FireLineTrace(const FVector& From, const FVector& Direction);

public:
	static const FName MuzzleSocketName;

private:
	int32 CurrentBulletCount;
};
