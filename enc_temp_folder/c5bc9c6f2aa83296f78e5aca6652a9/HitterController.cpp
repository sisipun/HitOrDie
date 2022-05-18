#include "HitterController.h"
#include "Hitter.h"

AHitterController::AHitterController()
{
}

void AHitterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG(LogTemp, Warning, TEXT("Set"));

	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Set Hit"));

		InputComponent->BindAction(TEXT("Jump"), IE_Pressed, Hitter, &ACharacter::Jump);
		InputComponent->BindAction(TEXT("Jump"), IE_Released, Hitter, &ACharacter::StopJumping);
	}

	InputComponent->BindAxis(TEXT("MoveX"), this, &AHitterController::MoveX);
	InputComponent->BindAxis(TEXT("MoveY"), this, &AHitterController::MoveY);

	InputComponent->BindAxis(TEXT("LookX"), this, &AHitterController::AddYawInput);
	InputComponent->BindAxis(TEXT("LookY"), this, &AHitterController::AddPitchInput);
}

void AHitterController::MoveX(float Scale)
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter && Scale != 0)
	{
		Hitter->AddMovementInput(Hitter->GetActorRightVector(), Scale);
	}
}

void AHitterController::MoveY(float Scale)
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter && Scale != 0)
	{
		Hitter->AddMovementInput(Hitter->GetActorForwardVector(), Scale);
	}
}