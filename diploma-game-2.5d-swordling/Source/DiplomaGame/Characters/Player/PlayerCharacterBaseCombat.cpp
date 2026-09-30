#include "Characters/Player/PlayerCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/StaminaComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void APlayerCharacterBase::Attack()
{
	StartAttack(AttackMontage, 1.0f, 0.0f, AttackStaminaCost);
}

void APlayerCharacterBase::StrongAttack()
{
	const bool bIsAirborne = GetCharacterMovement()->IsFalling();
	const float StunChance = bIsAirborne
		? AirStrongAttackStunChance
		: GroundStrongAttackStunChance;

	StartAttack(
		StrongAttackMontage,
		StrongAttackDamageMultiplier,
		StunChance,
		StrongAttackStaminaCost
	);
}

void APlayerCharacterBase::StartParry()
{
	if (bIsDead
		|| bIsRespawning
		|| bIsStunned
		|| bIsDoubleJumpSpinActive
		|| !StaminaComponent
		|| !StaminaComponent->CanConsumeStamina(ParryStaminaCost)
		|| !ParryMontage
		|| !GetWorld()
		|| !GetMesh())
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || AnimInstance->Montage_IsPlaying(ParryMontage))
	{
		return;
	}

	if (AnimInstance->Montage_Play(ParryMontage) <= 0.0f)
	{
		return;
	}

	bIsParryWindowActive = true;
	GetWorldTimerManager().SetTimer(
		ParryWindowTimerHandle,
		this,
		&APlayerCharacterBase::HandleParryWindowExpired,
		FMath::Max(ParryWindowDuration, UE_SMALL_NUMBER),
		false
	);
}

void APlayerCharacterBase::EndParryWindow()
{
	bIsParryWindowActive = false;
	GetWorldTimerManager().ClearTimer(ParryWindowTimerHandle);
}

void APlayerCharacterBase::HandleParryWindowExpired()
{
	if (!bIsParryWindowActive)
	{
		return;
	}

	bIsParryWindowActive = false;
	GetWorldTimerManager().ClearTimer(ParryWindowTimerHandle);

	if (StaminaComponent)
	{
		StaminaComponent->TryConsumeStamina(ParryStaminaCost);
	}
}

bool APlayerCharacterBase::TryParryEnemyBasicAttack()
{
	if (!bIsParryWindowActive || bIsDead || bIsStunned)
	{
		return false;
	}

	EndParryWindow();

	if (SuccessfulParrySound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SuccessfulParrySound,
			GetActorLocation()
		);
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Green,
			TEXT("Atak zablokowany")
		);
	}

	return true;
}

void APlayerCharacterBase::StartAttack(
	UAnimMontage* Montage,
	const float DamageMultiplier,
	const float StunChance,
	const float StaminaCost
)
{
	UWorld* World = GetWorld();
	if (bIsDead
		|| bIsRespawning
		|| bIsStunned
		|| bIsParryWindowActive
		|| !StaminaComponent
		|| !StaminaComponent->CanConsumeStamina(StaminaCost)
		|| !World
		|| !GetMesh())
	{
		return;
	}

	if (bIsDoubleJumpSpinActive)
	{
		if (GetCharacterMovement()->IsFalling())
		{
			return;
		}

		bIsDoubleJumpSpinActive = false;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (Montage == StrongAttackMontage
		&& AnimInstance
		&& AnimInstance->Montage_IsPlaying(Montage))
	{
		return;
	}

	const float CurrentTime = World->GetTimeSeconds();
	if (CurrentTime < NextAttackTime)
	{
		return;
	}

	if (!StaminaComponent->TryConsumeStamina(StaminaCost))
	{
		return;
	}

	NextAttackTime = CurrentTime + AttackCooldown;
	CurrentAttackDamageMultiplier = FMath::Max(0.0f, DamageMultiplier);
	CurrentAttackStunChance = FMath::Clamp(StunChance, 0.0f, 1.0f);
	bCurrentAttackStartedInAir = GetCharacterMovement()->IsFalling();

	if (Montage && Montage == StrongAttackMontage)
	{
		UCharacterMovementComponent* MovementComponent =
			GetCharacterMovement();
		if (!MovementComponent->IsFalling())
		{
			MovementComponent->StopMovementImmediately();
		}
	}

	if (Montage && AnimInstance)
	{
		if (AnimInstance->Montage_Play(Montage) > 0.0f)
		{
			return;
		}
	}

	PerformAttackHit();
}

bool APlayerCharacterBase::IsAttackingActor(
	const AActor* TargetActor
) const
{
	if (bIsDead
		|| bIsRespawning
		|| bIsStunned
		|| !IsValid(TargetActor)
		|| !GetMesh())
	{
		return false;
	}

	const UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	const bool bAttackMontageIsPlaying = AnimInstance
		&& ((AttackMontage
			&& AnimInstance->Montage_IsPlaying(AttackMontage))
			|| (StrongAttackMontage
				&& AnimInstance->Montage_IsPlaying(StrongAttackMontage)));
	if (!bAttackMontageIsPlaying)
	{
		return false;
	}

	FVector AttackDirection = GetMesh()->GetRightVector();
	AttackDirection.Z = 0.0f;
	FVector DirectionToTarget =
		TargetActor->GetActorLocation() - GetActorLocation();
	DirectionToTarget.Z = 0.0f;

	return AttackDirection.Normalize()
		&& DirectionToTarget.Normalize()
		&& FVector::DotProduct(AttackDirection, DirectionToTarget) > 0.5f;
}
