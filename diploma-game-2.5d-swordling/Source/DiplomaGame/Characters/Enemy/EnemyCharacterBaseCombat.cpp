#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Characters/Player/PlayerCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DiplomaGame.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEnemyCharacterBase::UpdateAttack()
{
	if (bIsDead
		|| bIsStunned
		|| bPressedJump
		|| GetCharacterMovement()->IsFalling()
		|| !bIsPlayerDetected
		|| !IsPlayerInAttackRange())
	{
		return;
	}

	const UWorld* World = GetWorld();
	if (!World || World->GetTimeSeconds() < NextAttackTime)
	{
		return;
	}

	PerformAttack();
	NextAttackTime = World->GetTimeSeconds() + AttackCooldown;
}

void AEnemyCharacterBase::PerformAttack()
{
	if (bIsDead
		|| bIsStunned
		|| bPressedJump
		|| GetCharacterMovement()->IsFalling()
		|| !IsPlayerInAttackRange())
	{
		return;
	}

	const bool bUseStrongAttack =
		StrongAttackMontage
		&& FMath::FRand() < FMath::Clamp(
			StrongAttackChance,
			0.0f,
			1.0f
		);

	UAnimMontage* SelectedMontage = bUseStrongAttack
		? StrongAttackMontage
		: AttackMontage;
	CurrentAttackDamageMultiplier = bUseStrongAttack
		? FMath::Max(StrongAttackDamageMultiplier, 0.0f)
		: 1.0f;
	CurrentAttackStunChance = bUseStrongAttack
		? FMath::Clamp(StrongAttackStunChance, 0.0f, 1.0f)
		: FMath::Clamp(BasicAttackStunChance, 0.0f, 1.0f);

	if (SelectedMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (AnimInstance->Montage_Play(SelectedMontage) > 0.0f)
			{
				return;
			}
		}
	}

	PerformAttackHit();
}

bool AEnemyCharacterBase::IsAttackingActor(
	const AActor* TargetActor
) const
{
	if (bIsDead
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

	return true;
}

void AEnemyCharacterBase::PerformAttackHit()
{
	if (bIsDead
		|| bIsStunned
		|| bPressedJump
		|| GetCharacterMovement()->IsFalling()
		|| !IsPlayerInAttackRange())
	{
		return;
	}

	UHealthComponent* PlayerHealthComponent =
		PlayerCharacter->FindComponentByClass<UHealthComponent>();

	if (!PlayerHealthComponent || PlayerHealthComponent->IsDead())
	{
		return;
	}

	if (bDrawAttackDebug)
	{
		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			AttackRange,
			24,
			FColor::Red,
			false,
			0.5f,
			0,
			2.0f
		);
	}

	if (PlayerCharacter->TryParryEnemyBasicAttack())
	{
		TryApplyStun(1.0f);
		return;
	}

	if (IsAttackingActor(PlayerCharacter)
		&& PlayerCharacter->IsAttackingActor(this))
	{
		PlayerCharacter->PlayWeaponClashSound();
		TryApplyStun(1.0f);
		PlayerCharacter->TryApplyStun(1.0f);

		UE_LOG(
			LogDiplomaGameCombat,
			Display,
			TEXT("Weapon clash between %s and %s"),
			*GetName(),
			*PlayerCharacter->GetName()
		);
		return;
	}

	const float Damage = AttackDamage * CurrentAttackDamageMultiplier;
	PlayerHealthComponent->ApplyDamage(Damage);
	PlayerCharacter->TryApplyStun(CurrentAttackStunChance);

	UE_LOG(
		LogDiplomaGameCombat,
		Display,
		TEXT("%s attacked %s for %.1f damage"),
		*GetName(),
		*PlayerCharacter->GetName(),
		Damage
	);
}

bool AEnemyCharacterBase::IsPlayerInAttackRange() const
{
	if (!IsValid(PlayerCharacter) || PlayerCharacter->IsDead())
	{
		return false;
	}

	return FVector::DistSquared(
		GetActorLocation(),
		PlayerCharacter->GetActorLocation()
	) <= FMath::Square(AttackRange);
}
