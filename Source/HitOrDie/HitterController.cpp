#include "HitterController.h"

#include "GameFramework/Character.h"

AHitterController::AHitterController()
{
}

void AHitterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHitterController::Jump);
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AHitterController::StopJumping);

	InputComponent->BindAxis(TEXT("MoveX"), this, &AHitterController::MoveX);
	InputComponent->BindAxis(TEXT("MoveY"), this, &AHitterController::MoveY);

	InputComponent->BindAxis(TEXT("LookX"), this, &AHitterController::AddYawInput);
	InputComponent->BindAxis(TEXT("LookY"), this, &AHitterController::AddPitchInput);
}

void AHitterController::Jump()
{
	ACharacter* ControlledPawn = Cast<ACharacter>(GetPawn());
	if (ControlledPawn)
	{
		ControlledPawn->Jump();
	}
}

void AHitterController::StopJumping()
{
	ACharacter* ControlledPawn = Cast<ACharacter>(GetPawn());
	if (ControlledPawn)
	{
		ControlledPawn->StopJumping();
	}
}

void AHitterController::MoveX(float Scale)
{
	ACharacter* ControlledPawn = Cast<ACharacter>(GetPawn());
	if (ControlledPawn && Scale != 0)
	{
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), Scale);
	}
}

void AHitterController::MoveY(float Scale)
{
	ACharacter* ControlledPawn = Cast<ACharacter>(GetPawn());
	if (ControlledPawn && Scale != 0)
	{
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), Scale);
	}
}