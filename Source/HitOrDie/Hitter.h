#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hitter.generated.h"

class UCameraComponent;
class SkeletalMeshComponent;

UCLASS()
class HITORDIE_API AHitter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	USkeletalMeshComponent* Mesh1P;

public:
	AHitter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
