#include "HitterController.h"
#include "HitterController.h"

#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"

#include "Hitter.h"

AHitterController::AHitterController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHitterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AHitterController::Jump);
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AHitterController::StopJumping);

	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AHitterController::Fire);

	InputComponent->BindAxis(TEXT("MoveX"), this, &AHitterController::MoveX);
	InputComponent->BindAxis(TEXT("MoveY"), this, &AHitterController::MoveY);

	InputComponent->BindAxis(TEXT("LookX"), this, &AHitterController::LookX);
	InputComponent->BindAxis(TEXT("LookY"), this, &AHitterController::LookY);
}

void AHitterController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
	AHitter* Hitter = Cast<AHitter>(PossessedPawn);
	if (Hitter)
	{
		Hitter->OnDead.AddDynamic(this, &AHitterController::Auth_OnDead);
	}
}

void AHitterController::Auth_OnDead()
{
	check(HasAuthority());

	GetPawn()->Destroy();
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

void AHitterController::Jump()
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter)
	{
		Hitter->Jump();
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

void AHitterController::Fire()
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter)
	{
		Hitter->Fire();
	}
}

void AHitterController::MoveX(float Scale)
{
	if (Scale != 0.0)
	{
		AHitter* Hitter = Cast<AHitter>(GetPawn());
		if (Hitter)
		{
			Hitter->MoveX(Scale);
		}
	}
}

void AHitterController::MoveY(float Scale)
{
	if (Scale != 0.0)
	{
		AHitter* Hitter = Cast<AHitter>(GetPawn());
		if (Hitter)
		{
			Hitter->MoveY(Scale);
		}
	}
}

void AHitterController::LookX(float Scale)
{
	if (Scale != 0.0) 
	{
		AddYawInput(Scale);
		AHitter* Hitter = Cast<AHitter>(GetPawn());
		if (Hitter)
		{
			Hitter->Server_SyncCameraRotation();
		}
	}
}

void AHitterController::LookY(float Scale)
{
	if (Scale != 0.0)
	{
		AddPitchInput(Scale);
		AHitter* Hitter = Cast<AHitter>(GetPawn());
		if (Hitter)
		{
			Hitter->Server_SyncCameraRotation();
		}
	}
}