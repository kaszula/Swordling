#include "Characters/Player/PlayerCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "DiplomaGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

void APlayerCharacterBase::RespawnAtDeathLocation()
{
	if (!GetWorld() || bIsGameOver || !HealthComponent)
	{
		return;
	}

	SetActorTransform(
		DeathTransform,
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
	bIsDead = false;
	bDeathPresentationStarted = false;
	bIsRespawning = true;
	HealthComponent->ResetHealth();
	if (StaminaComponent)
	{
		StaminaComponent->ResetStamina();
	}
	HealthComponent->SetInvulnerable(true);
	StopRunningSound();
	StopHitReact();
	ClearHitFlash();
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	StartRespawnAnimation();
}

void APlayerCharacterBase::StartRespawnAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()
		? GetMesh()->GetAnimInstance()
		: nullptr;

	if (!AnimInstance
		|| !RespawnMontage
		|| AnimInstance->Montage_Play(RespawnMontage) <= 0.0f)
	{
		if (RespawnFallbackDuration <= 0.0f)
		{
			CompleteRespawn();
			return;
		}

		GetWorldTimerManager().SetTimer(
			RespawnTimerHandle,
			this,
			&APlayerCharacterBase::CompleteRespawn,
			RespawnFallbackDuration,
			false
		);
		return;
	}

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(
		this,
		&APlayerCharacterBase::HandleRespawnMontageEnded
	);
	AnimInstance->Montage_SetEndDelegate(
		MontageEndedDelegate,
		RespawnMontage
	);
}

void APlayerCharacterBase::HandleRespawnMontageEnded(
	UAnimMontage* Montage,
	const bool bInterrupted
)
{
	if (Montage != RespawnMontage)
	{
		return;
	}

	CompleteRespawn();
}

void APlayerCharacterBase::CompleteRespawn()
{
	if (!bIsRespawning || bIsGameOver)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
	bIsRespawning = false;

	if (HealthComponent)
	{
		HealthComponent->SetInvulnerable(false);
	}

	SetEnemiesIgnoredDuringDeath(false);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	OnRespawnCompleted.Broadcast(RemainingLives);
}

void APlayerCharacterBase::BeginGameOver()
{
	if (bIsGameOver || !GetWorld())
	{
		return;
	}

	bIsGameOver = true;
	bIsRespawning = false;
	GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	if (APlayerController* PlayerController =
		Cast<APlayerController>(Controller))
	{
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetIgnoreLookInput(true);
	}

	OnGameOver.Broadcast();

	UE_LOG(
		LogDiplomaGameCombat,
		Display,
		TEXT("Game over. Waiting for the player's menu choice")
	);
}
