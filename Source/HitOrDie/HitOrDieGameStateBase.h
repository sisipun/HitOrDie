#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "HitOrDieGameStateBase.generated.h"

class ASoundEmitter;

UCLASS()
class HITORDIE_API AHitOrDieGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	bool GetPossibleAction() const;

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<ASoundEmitter> SoundEmitter;
};
