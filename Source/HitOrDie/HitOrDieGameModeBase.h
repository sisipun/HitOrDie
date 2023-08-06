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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Rules)
	int MinPlayersCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Rules)
	int MaxPlayersCount;

	UPROPERTY(EditDefaultsOnly, Category = Rules)
	float CooldownDuration;

private:
	UPROPERTY(Transient)
	TObjectPtr<ASoundEmitter> SoundEmitter;

public:
	AHitOrDieGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	bool Auth_PerformAction(AHitterController* Hitter);

	void Auth_OnKilled(AHitterController* Hitter, UPlayer* Hitted);

	float GetCooldownDuration() const;

private:
	TMap<FString, int32> PlayersScore;
};
