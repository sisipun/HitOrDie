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

	InputComponent->BindAxis(TEXT("LookX"), this, &AHitterController::AddYawInput);
	InputComponent->BindAxis(TEXT("LookY"), this, &AHitterController::AddPitchInput);
}

void AHitterController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
}

void AHitterController::Auth_OnDead()
{
	check(HasAuthority());

	UE_LOG(LogTemp, Warning, TEXT("RESTART"));
	GetPawn()->Destroy();
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

void AHitterController::Jump()
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter && !Hitter->IsDead())
	{
		Hitter->Jump();
	}
}

void AHitterController::Fire()
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter && !Hitter->IsDead())
	{
		Hitter->Fire();
	}
}

void AHitterController::StopJumping()
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter && !Hitter->IsDead())
	{
		Hitter->StopJumping();
	}
}

void AHitterController::MoveX(float Scale)
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter && !Hitter->IsDead() && Scale != 0)
	{
		Hitter->AddMovementInput(Hitter->GetActorRightVector(), Scale);
	}
}

void AHitterController::MoveY(float Scale)
{
	AHitter* Hitter = Cast<AHitter>(GetPawn());
	if (Hitter && !Hitter->IsDead() && Scale != 0)
	{
		Hitter->AddMovementInput(Hitter->GetActorForwardVector(), Scale);
	}
}