#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "SoundEmitter.h"

#include "HitOrDieGameModeBase.generated.h"

class ASoundEmitter;
class AHitterController;

UCLASS()
class HITORDIE_API AHitOrDieGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MinPlayersCount;

private:
	UPROPERTY(Transient)
	TObjectPtr<ASoundEmitter> SoundEmitter;

public:
	AHitOrDieGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	bool Auth_PerformAction(AHitterController* Hitter, EActionType Action);

	void Auth_OnKilled(TObjectPtr<AHitterController> Hitter, UPlayer* Hitted);

protected:
	virtual void BeginPlay() override;

private:
	TMap<FString, int32> PlayersScore;
};
