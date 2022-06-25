#include "HitterController.h"

#include "GameFramework/Character.h"

#include "Hitter.h"

AHitterController::AHitterController()
{
}

void AHitterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHitterController::Jump);
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AHitterController::StopJumping);

	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AHitterController::Fire);

	InputComponent->BindAxis(TEXT("MoveX"), this, &AHitterController::MoveX);
	InputComponent->BindAxis(TEXT("MoveY"), this, &AHitterController::MoveY);

	InputComponent->BindAxis(TEXT("LookX"), this, &AHitterController::AddYawInput);
	InputComponent->BindAxis(TEXT("LookY"), this, &AHitterController::AddPitchInput);
}

void AHitterController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
	UE_LOG(LogTemp, Warning, TEXT("POSS"));
}

void AHitterController::Jump()
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter)
	{
		UE_LOG(LogTemp, Warning, TEXT("J EXEC"));
		Hitter->Jump();
	}
}

void AHitterController::Fire()
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter)
	{
		Hitter->Fire();
	}
}

void AHitterController::StopJumping()
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter)
	{
		Hitter->StopJumping();
	}
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