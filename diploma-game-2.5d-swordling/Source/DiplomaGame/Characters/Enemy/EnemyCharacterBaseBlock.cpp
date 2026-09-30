#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

bool AEnemyCharacterBase::TryBlockPlayerGroundAttack()
{
	if (bIsDead
		|| bIsStunned
		|| GetCharacterMovement()->IsFalling())
	{
		return false;
	}

	if (bIsBlocking)
	{
		return true;
	}

	const float ClampedBlockChance = FMath::Clamp(
		BlockChance,
		0.0f,
		1.0f
	);
	if (ClampedBlockChance <= 0.0f
		|| FMath::FRand() >= ClampedBlockChance)
	{
		return false;
	}

	bIsBlocking = true;
	StopChasing();

	UAnimInstance* AnimInstance = GetMesh()
		? GetMesh()->GetAnimInstance()
		: nullptr;
	if (!AnimInstance || !BlockMontage)
	{
		bIsBlocking = false;
		return true;
	}

	AnimInstance->StopAllMontages(0.0f);
	if (AnimInstance->Montage_Play(BlockMontage) <= 0.0f)
	{
		bIsBlocking = false;
		return true;
	}

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(
		this,
		&AEnemyCharacterBase::HandleBlockMontageEnded
	);
	AnimInstance->Montage_SetEndDelegate(
		MontageEndedDelegate,
		BlockMontage
	);

	return true;
}

bool AEnemyCharacterBase::IsBlocking() const
{
	return bIsBlocking;
}

void AEnemyCharacterBase::HandleBlockMontageEnded(
	UAnimMontage* Montage,
	const bool bInterrupted
)
{
	if (Montage == BlockMontage)
	{
		bIsBlocking = false;
	}
}
