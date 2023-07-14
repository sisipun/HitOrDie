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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxPlayersCount;

private:
	UPROPERTY(Transient)
	TObjectPtr<ASoundEmitter> SoundEmitter;

public:
	AHitOrDieGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	bool Auth_PerformAction(AHitterController* Hitter, EActionType Action);

	void Auth_OnKilled(TObjectPtr<AHitterController> Hitter, UPlayer* Hitted);

private:
	TMap<FString, int32> PlayersScore;
};
