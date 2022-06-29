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

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USkeletalMeshComponent> Mesh3P;

	UPROPERTY(EditAnywhere, Category = Stats)
	float MaxHealth;

	UPROPERTY(Replicated, VisibleAnywhere, Category = Stats)
	float Health;

public:
	AHitter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void Fire();

	void Hit(float Value);

protected:
	virtual void BeginPlay() override;

private:
	void SpawnWeapon();

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
