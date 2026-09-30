#include "Characters/Player/PlayerCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Characters/Enemy/EnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "DiplomaGame.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"

void APlayerCharacterBase::HandleDeath()
{
	if (bIsDead || bIsGameOver)
	{
		return;
	}

	DeathTransform = GetActorTransform();
	bIsDead = true;
	bIsStunned = false;
	RemainingLives = FMath::Max(RemainingLives - 1, 0);
	OnLivesChanged.Broadcast(RemainingLives);

	if (HealthComponent)
	{
		HealthComponent->SetInvulnerable(true);
	}

	StopRunningSound();
	GetWorldTimerManager().ClearTimer(HitReactTimerHandle);
	ClearHitFlash();
	EndParryWindow();
	bIsDoubleJumpSpinActive = false;
	GetWorldTimerManager().ClearTimer(StunTimerHandle);
	StopJumping();
	SetEnemiesIgnoredDuringDeath(true);

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent->IsFalling())
	{
		FVector FallingVelocity = MovementComponent->Velocity;
		FallingVelocity.X = 0.0f;
		FallingVelocity.Y = 0.0f;
		MovementComponent->Velocity = FallingVelocity;
		return;
	}

	FinishDeath();
}

void APlayerCharacterBase::FinishDeath()
{
	if (bDeathPresentationStarted)
	{
		return;
	}

	bDeathPresentationStarted = true;
	OnLifeLossStarted.Broadcast(RemainingLives);
	GetCharacterMovement()->StopMovementImmediately();

	float DeathMontageDuration = 0.0f;
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);
		if (DeathMontage)
		{
			DeathMontageDuration = AnimInstance->Montage_Play(
				DeathMontage
			);
		}
	}

	UE_LOG(
		LogDiplomaGameCombat,
		Display,
		TEXT("%s died. Lives remaining: %d"),
		*GetName(),
		RemainingLives
	);

	if (RemainingLives <= 0)
	{
		const float DeathPresentationDuration = DeathMontageDuration > 0.0f
			? DeathMontageDuration
			: FMath::Max(DeathRestartDelay, 0.0f);
		const float GameOverDelay = DeathPresentationDuration
			+ FMath::Max(GameOverScreenDelay, 0.0f);

		if (GameOverDelay <= 0.0f)
		{
			BeginGameOver();
			return;
		}

		GetWorldTimerManager().SetTimer(
			RespawnTimerHandle,
			this,
			&APlayerCharacterBase::BeginGameOver,
			GameOverDelay,
			false
		);
		return;
	}

	if (DeathRestartDelay <= 0.0f)
	{
		RespawnAtDeathLocation();
		return;
	}

	GetWorldTimerManager().SetTimer(
		RespawnTimerHandle,
		this,
		&APlayerCharacterBase::RespawnAtDeathLocation,
		DeathRestartDelay,
		false
	);
}

void APlayerCharacterBase::SetEnemiesIgnoredDuringDeath(
	const bool bShouldIgnore
)
{
	UWorld* World = GetWorld();
	UCapsuleComponent* PlayerCapsule = GetCapsuleComponent();
	if (!World || !PlayerCapsule)
	{
		return;
	}

	for (TActorIterator<AEnemyCharacterBase> EnemyIterator(World);
		EnemyIterator;
		++EnemyIterator)
	{
		AEnemyCharacterBase* Enemy = *EnemyIterator;
		if (!IsValid(Enemy))
		{
			continue;
		}

		PlayerCapsule->IgnoreActorWhenMoving(Enemy, bShouldIgnore);
		Enemy->GetCapsuleComponent()->IgnoreActorWhenMoving(
			this,
			bShouldIgnore
		);
	}
}
