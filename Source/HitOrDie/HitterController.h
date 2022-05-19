#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "HitterController.generated.h"

class AHitter;

UCLASS()
class HITORDIE_API AHitterController : public APlayerController
{
	GENERATED_BODY()

public:
	AHitterController();

	virtual void SetupInputComponent() override;

private:
	void Jump();

	void StopJumping();

	void MoveX(float Scale);
	
	void MoveY(float Scale);
};
