#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

class AHitter;

UCLASS()
class HITORDIE_API AAbility : public AActor
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void Auth_Use();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float ReloadDuration;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Stats)
	bool bReloading;

	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<AHitter> Hitter;

public:
	AAbility();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AttachTo(AHitter* AttachedHitter);

	void Auth_TryUse();

	bool CanUse() const;

protected:
	virtual void BeginPlay() override;

private:
	void Auth_OnAbilityReloadFinished();

private:
	FTimerHandle AbilityReloadTimer;
};
