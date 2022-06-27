#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "Hitter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class AWeapon;

UCLASS()
class HITORDIE_API AHitter : public ACharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void Server_SpawnBullet(TSubclassOf<ABullet> BulletType, FTransform SpawnLocation);

public:
	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	AWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AWeapon> WeaponType;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

public:
	AHitter();

	void Fire();

protected:
	virtual void BeginPlay() override;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
