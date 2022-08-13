#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "SoundEmitter.h"

#include "HitOrDieGameStateBase.generated.h"

UCLASS()
class HITORDIE_API AHitOrDieGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FTiming> GetPossibleActions(float HalfPeriod) const;

	UFUNCTION(BlueprintCallable)
	bool PerformAction(EActionType Action);

private:
	UPROPERTY(Transient)
	TObjectPtr<ASoundEmitter> SoundEmitter;

public:
	AHitOrDieGameStateBase();

	void Auth_OnKilled(UPlayer* Hitter, UPlayer* Hitted);

protected:
	virtual void BeginPlay() override;

private:
	TMap<FString, int32> PlayersScore;
};
