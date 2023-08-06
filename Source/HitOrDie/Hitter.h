#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "SoundEmitter.h"

#include "Hitter.generated.h"

class AAbility;
class AHitterController;
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
	void Server_UseAbility();

	UFUNCTION(Server, Reliable)
	void Server_SyncCameraRotation();

	UFUNCTION(BlueprintCallable)
	void Auth_Hit(AHitterController* Hitter, float Value);

	UFUNCTION(BlueprintCallable)
	void Auth_Heal(float Value);

private:
	UFUNCTION()
	void OnRep_bDead();

public:
	UPROPERTY(BlueprintAssignable)
	FDeadSignature OnDead;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USkeletalMeshComponent> Mesh3P;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float MaxHealth;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Stats)
	float Health;
	
	UPROPERTY(ReplicatedUsing=OnRep_bDead, BlueprintReadOnly, Category = Stats)
	bool bDead;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Stats)
	bool bActionCooldown;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Weapon)
	AWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AWeapon> WeaponType;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Ability)
	AAbility* Ability;

	UPROPERTY(EditAnywhere, Category = Ability)
	TSubclassOf<AAbility> AbilityType;

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

	void UseAbility();

	bool IsDead() const;

	TObjectPtr<USkeletalMeshComponent> GetMesh() const;

protected:
	virtual void BeginPlay() override;

private:
	bool Auth_TryAction();

	void Auth_OnActionCooldownFinished();

	void Auth_OnDead();

	void SpawnWeapon();

	void SpawnAbility();

public:
	static const FName GripSocketName;

private:
	FTimerHandle ActionCooldownTimer;
};
