#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "HitOrDieGameStateBase.generated.h"

class ASoundEmitter;
enum class EActionType : uint8;

UCLASS()
class HITORDIE_API AHitOrDieGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	EActionType GetPossibleAction() const;

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<ASoundEmitter> SoundEmitter;
};
