#include "Characters/Player/PlayerCharacterBase.h"

#include "EnhancedInputComponent.h"

void APlayerCharacterBase::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent
)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EnhancedInputComponent)
	{
		return;
	}

	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerCharacterBase::Move
		);

		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Completed,
			this,
			&APlayerCharacterBase::StopMove
		);

		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Canceled,
			this,
			&APlayerCharacterBase::StopMove
		);
	}

	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&APlayerCharacterBase::StartJump
		);

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&APlayerCharacterBase::StopJump
		);
	}

	if (AttackAction)
	{
		EnhancedInputComponent->BindAction(
			AttackAction,
			ETriggerEvent::Started,
			this,
			&APlayerCharacterBase::Attack
		);
	}

	if (StrongAttackAction)
	{
		EnhancedInputComponent->BindAction(
			StrongAttackAction,
			ETriggerEvent::Started,
			this,
			&APlayerCharacterBase::StrongAttack
		);
	}

	if (ParryAction)
	{
		EnhancedInputComponent->BindAction(
			ParryAction,
			ETriggerEvent::Started,
			this,
			&APlayerCharacterBase::StartParry
		);
	}
}
