#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "HitterController.generated.h"

class AHitter;

UCLASS()
class HITORDIE_API AHitterController : public APlayerController
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void Auth_OnDead();

public:
	AHitterController();

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* PossessedPawn) override;

private:
	void Jump();

	void Fire();
	
	void UseAbility();

	void StopJumping();

	void MoveX(float Scale);
	
	void MoveY(float Scale);

	void LookX(float Scale);

	void LookY(float Scale);
};
