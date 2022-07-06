#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "HitOrDieGameStateBase.generated.h"

class ASoundEmitter;
enum class ActionType : uint8;

UCLASS()
class HITORDIE_API AHitOrDieGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ActionType GetPossibleAction() const;

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<ASoundEmitter> SoundEmitter;
};
