#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "SoundEmitter.h"

#include "HitOrDieGameStateBase.generated.h"

class AHitterController;
class ASoundEmitter;

UCLASS()
class HITORDIE_API AHitOrDieGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool Auth_PerformAction(AHitterController* Hitter, EActionType Action);

	UFUNCTION(BlueprintPure)
	TArray<FTiming> GetPossibleActions(AHitterController* Hitter, float PeriodBefore, float PeriodAfter) const;

	UFUNCTION(BlueprintPure)
	float GetPlaybackValue() const;

	UFUNCTION(BlueprintPure)
	int GetCountdownValue() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<ASoundEmitter> SoundEmitter;

public:
	AHitOrDieGameStateBase();

	void Auth_OnKilled(TObjectPtr<AHitterController> Hitter, UPlayer* Hitted);

protected:
	virtual void BeginPlay() override;

private:
	TMap<FString, int32> PlayersScore;
};
