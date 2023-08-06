#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "SoundEmitter.h"

#include "HitOrDieGameStateBase.generated.h"

class AHitterController;

UCLASS()
class HITORDIE_API AHitOrDieGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	TArray<FTiming> GetPossibleActions(AHitterController* Hitter, float PeriodBefore, float PeriodAfter) const;

	UFUNCTION(BlueprintPure)
	float GetPlaybackValue() const;

	UFUNCTION(BlueprintPure)
	int32 GetCountdownValue() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<ASoundEmitter> SoundEmitter;

public:
	AHitOrDieGameStateBase();

protected:
	virtual void BeginPlay() override;
};
