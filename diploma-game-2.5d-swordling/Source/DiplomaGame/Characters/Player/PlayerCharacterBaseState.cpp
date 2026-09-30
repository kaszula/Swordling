#include "Characters/Player/PlayerCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/HealthComponent.h"
#include "DiplomaGameGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

bool APlayerCharacterBase::TryApplyStun(const float StunChance)
{
	if (bIsDead || bIsRespawning || bIsStunned)
	{
		return false;
	}

	const float ClampedChance = FMath::Clamp(StunChance, 0.0f, 1.0f);
	if (ClampedChance <= 0.0f || FMath::FRand() >= ClampedChance)
	{
		return false;
	}

	StartStun();
	return true;
}

void APlayerCharacterBase::HandleDamageApplied(const float DamageAmount)
{
	if (UDiplomaGameGameInstance* GameInstance =
		GetGameInstance<UDiplomaGameGameInstance>())
	{
		GameInstance->RecordPlayerDamage(DamageAmount);
	}
}

void APlayerCharacterBase::HandleDamaged()
{
	if (bIsDead
		|| bIsRespawning
		|| !HealthComponent
		|| HealthComponent->IsDead()
		|| !GetMesh()
		|| !GetWorld())
	{
		return;
	}

	if (HitFlashMaterial)
	{
		GetMesh()->SetOverlayMaterial(HitFlashMaterial);
		GetWorldTimerManager().SetTimer(
			HitFlashTimerHandle,
			this,
			&APlayerCharacterBase::ClearHitFlash,
			FMath::Max(HitFlashDuration, UE_SMALL_NUMBER),
			false
		);
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!bIsStunned && AnimInstance && HitReactMontage)
	{
		GetWorldTimerManager().ClearTimer(HitReactTimerHandle);
		if (AnimInstance->Montage_Play(HitReactMontage) > 0.0f)
		{
			GetWorldTimerManager().SetTimer(
				HitReactTimerHandle,
				this,
				&APlayerCharacterBase::StopHitReact,
				FMath::Max(HitReactDuration, UE_SMALL_NUMBER),
				false
			);
		}
	}
}

void APlayerCharacterBase::StopHitReact()
{
	if (GetMesh())
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Stop(0.05f, HitReactMontage);
		}
	}
	GetWorldTimerManager().ClearTimer(HitReactTimerHandle);
}

void APlayerCharacterBase::ClearHitFlash()
{
	if (GetMesh())
	{
		GetMesh()->SetOverlayMaterial(nullptr);
	}
	GetWorldTimerManager().ClearTimer(HitFlashTimerHandle);
}

void APlayerCharacterBase::StartStun()
{
	if (bIsDead || bIsStunned || !GetWorld())
	{
		return;
	}

	bIsStunned = true;
	StopRunningSound();
	EndParryWindow();
	StopJumping();
	GetCharacterMovement()->StopMovementImmediately();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);
		if (StunMontage)
		{
			const float PlayRate =
				StunMontage->GetPlayLength() / StunDuration;
			AnimInstance->Montage_Play(StunMontage, PlayRate);
		}
	}

	GetWorldTimerManager().SetTimer(
		StunTimerHandle,
		this,
		&APlayerCharacterBase::EndStun,
		StunDuration,
		false
	);
}

void APlayerCharacterBase::EndStun()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Stop(0.1f, StunMontage);
	}

	bIsStunned = false;
}
