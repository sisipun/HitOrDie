#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "HitOrDieGameModeBase.generated.h"

class ASoundEmitter;

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

protected:
	virtual void BeginPlay() override;
};
