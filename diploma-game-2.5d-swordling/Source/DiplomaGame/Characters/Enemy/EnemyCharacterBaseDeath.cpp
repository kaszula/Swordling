#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/Player/PlayerCharacterBase.h"
#include "DiplomaGame.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEnemyCharacterBase::HandleDeath()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->RecordFangKill();
	}
	bIsBlocking = false;
	bIsStunned = false;
	GetWorldTimerManager().ClearTimer(HitReactTimerHandle);
	ClearHitFlash();
	SetActorTickEnabled(false);
	GetWorldTimerManager().ClearTimer(DetectionTimerHandle);
	GetWorldTimerManager().ClearTimer(StunTimerHandle);
	SetPlayerDetectionState(false);

	UE_LOG(
		LogDiplomaGameCombat,
		Display,
		TEXT("%s died"),
		*GetName()
	);

	GetCapsuleComponent()->SetCollisionResponseToChannel(
		ECC_Pawn,
		ECR_Ignore
	);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent->IsFalling()
		|| !HasGroundSupportBelowCenter())
	{
		MovementComponent->SetMovementMode(MOVE_Falling);
		GetWorldTimerManager().SetTimer(
			DeathLandingTimerHandle,
			this,
			&AEnemyCharacterBase::FinishDeath,
			FMath::Max(DeathLandingTimeout, UE_SMALL_NUMBER),
			false
		);
		return;
	}

	FinishDeath();
}

void AEnemyCharacterBase::FinishDeath()
{
	if (bDeathPresentationStarted)
	{
		return;
	}

	bDeathPresentationStarted = true;
	GetWorldTimerManager().ClearTimer(DeathLandingTimerHandle);
	SetActorEnableCollision(false);
	GetCapsuleComponent()->SetCollisionEnabled(
		ECollisionEnabled::NoCollision
	);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	float DeathAnimationDuration = 0.0f;
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);
		if (DeathMontage)
		{
			DeathAnimationDuration = AnimInstance->Montage_Play(
				DeathMontage
			);
		}
	}

	const float RemainingLifeTime =
		DeathAnimationDuration
		+ FMath::Max(DeathDisappearDelay, 0.0f);
	if (RemainingLifeTime <= 0.0f)
	{
		Destroy();
		return;
	}

	SetLifeSpan(RemainingLifeTime);
}
