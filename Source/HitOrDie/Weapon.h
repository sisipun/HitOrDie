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

public:
	UFUNCTION(BlueprintCallable)
	FTransform GetMuzzleTransform() const;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleDefaultsOnly, Category = Body)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float Power;

	UPROPERTY(EditAnywhere, Category = Ammunition)
	int32 MaxBulletCount;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Ammunition)
	int32 BulletCount;

	UPROPERTY(Transient)
	TObjectPtr<AHitter> Hitter;

public:	
	AWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void AttachTo(AHitter* AttachedHitter);

	void Auth_Fire(const FVector& From, const FVector& Direction);

protected:
	virtual void BeginPlay() override;

private:
	void Auth_FireLineTrace(const FVector& From, const FVector& Direction);

public:
	static const FName MuzzleSocketName;
};
