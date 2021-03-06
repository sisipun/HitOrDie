#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "Hitter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class AWeapon;
class ABullet;
class ASoundEmitter;

UCLASS()
class HITORDIE_API AHitter : public ACharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void Server_SpawnBullet(TSubclassOf<ABullet> BulletType, FTransform SpawnLocation);

private:
	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_bDead();

protected:
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float MaxHealth;

	UPROPERTY(ReplicatedUsing=OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	float Health;
	
	UPROPERTY(ReplicatedUsing=OnRep_bDead, VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	bool bDead;

public:
	AHitter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Fire();

	void Auth_Hit(UPlayer* Hitter, float Value);

	TObjectPtr<USkeletalMeshComponent> GetMesh() const;

	bool IsDead() const;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnWeapon();

public:
	static const FName GripSocketName;
};
