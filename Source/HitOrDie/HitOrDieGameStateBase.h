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
	UFUNCTION(BlueprintCallable)
	EActionType GetPossibleAction() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<ASoundEmitter> SoundEmitter;

public:
	AHitOrDieGameStateBase();

	

protected:
	virtual void BeginPlay() override;
};
