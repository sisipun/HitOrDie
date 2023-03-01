#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "Hitter.generated.h"

class ABullet;
class AHitterController;
class ASoundEmitter;
class AWeapon;
class UCameraComponent;
class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeadSignature);

UCLASS()
class HITORDIE_API AHitter : public ACharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void Server_Fire();

	UFUNCTION(Server, Reliable)
	void Server_SyncCameraRotation();

private:
	UFUNCTION()
	void OnRep_bDead();

public:
	UPROPERTY(BlueprintAssignable)
	FDeadSignature OnDead;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = Weapon)
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

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = Stats)
	float Health;
	
	UPROPERTY(ReplicatedUsing=OnRep_bDead, BlueprintReadOnly, VisibleAnywhere, Category = Stats)
	bool bDead;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = Stats)
	bool bActionCooldown;

public:
	AHitter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Jump() override;

	virtual void StopJumping() override;

	void MoveX(float Scale);

	void MoveY(float Scale);

	void Fire();

	void Auth_Hit(TObjectPtr<AHitterController> Hitter, float Value);

	bool IsDead() const;

	TObjectPtr<USkeletalMeshComponent> GetMesh() const;

protected:
	virtual void BeginPlay() override;

private:
	void Auth_SpawnBullet(TSubclassOf<ABullet> BulletType, FTransform SpawnLocation);

	void Auth_OnActionCooldownFinished();

	void Auth_OnDead();

	void SpawnWeapon();

public:
	static const FName GripSocketName;

private:
	FTimerHandle ActionCooldownTimer;
};
