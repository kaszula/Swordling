#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/HealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

bool AEnemyCharacterBase::TryApplyStun(const float StunChance)
{
	if (bIsDead || bIsStunned)
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

bool AEnemyCharacterBase::IsStunned() const
{
	return bIsStunned;
}

void AEnemyCharacterBase::HandleDamaged()
{
	UWorld* World = GetWorld();
	if (bIsDead
		|| !HealthComponent
		|| HealthComponent->IsDead()
		|| !GetMesh()
		|| !World)
	{
		return;
	}

	if (HitFlashMaterial)
	{
		GetMesh()->SetOverlayMaterial(HitFlashMaterial);
		World->GetTimerManager().SetTimer(
			HitFlashTimerHandle,
			this,
			&AEnemyCharacterBase::ClearHitFlash,
			FMath::Max(HitFlashDuration, UE_SMALL_NUMBER),
			false
		);
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!bIsStunned && AnimInstance && HitReactMontage)
	{
		World->GetTimerManager().ClearTimer(HitReactTimerHandle);
		if (AnimInstance->Montage_Play(HitReactMontage) > 0.0f)
		{
			World->GetTimerManager().SetTimer(
				HitReactTimerHandle,
				this,
				&AEnemyCharacterBase::StopHitReact,
				FMath::Max(HitReactDuration, UE_SMALL_NUMBER),
				false
			);
		}
	}
}

void AEnemyCharacterBase::StopHitReact()
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

void AEnemyCharacterBase::ClearHitFlash()
{
	if (GetMesh())
	{
		GetMesh()->SetOverlayMaterial(nullptr);
	}
	GetWorldTimerManager().ClearTimer(HitFlashTimerHandle);
}

void AEnemyCharacterBase::StartStun()
{
	UWorld* World = GetWorld();
	if (bIsDead || bIsStunned || !World)
	{
		return;
	}

	bIsStunned = true;
	bIsBlocking = false;
	StopChasing();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);
		if (StunMontage)
		{
			const float SafeStunDuration = FMath::Max(
				StunDuration,
				UE_SMALL_NUMBER
			);
			const float PlayRate =
				StunMontage->GetPlayLength() / SafeStunDuration;
			AnimInstance->Montage_Play(StunMontage, PlayRate);
		}
	}

	World->GetTimerManager().SetTimer(
		StunTimerHandle,
		this,
		&AEnemyCharacterBase::EndStun,
		FMath::Max(StunDuration, UE_SMALL_NUMBER),
		false
	);
}

void AEnemyCharacterBase::EndStun()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Stop(0.1f, StunMontage);
	}

	bIsStunned = false;
}
