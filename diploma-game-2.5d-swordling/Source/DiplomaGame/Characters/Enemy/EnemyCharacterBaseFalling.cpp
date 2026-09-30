#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Characters/Player/PlayerCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEnemyCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	APlayerCharacterBase* LandedPlayer = Cast<APlayerCharacterBase>(
		Hit.GetActor()
	);
	if (!bIsDead && IsValid(LandedPlayer))
	{
		BounceOffPlayer(LandedPlayer);
		return;
	}

	AEnemyCharacterBase* LandedEnemy = Cast<AEnemyCharacterBase>(
		Hit.GetActor()
	);
	if (!bIsDead && IsValid(LandedEnemy) && LandedEnemy != this)
	{
		BeginPassingThroughEnemy(LandedEnemy);
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		return;
	}

	if (!HasGroundSupportBelowCenter())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		return;
	}

	RestoreEnemyCollisionAfterLanding();

	if (bIsDead)
	{
		FinishDeath();
	}
}

void AEnemyCharacterBase::BeginPassingThroughEnemy(
	AEnemyCharacterBase* OtherEnemy
)
{
	const TWeakObjectPtr<AEnemyCharacterBase> OtherEnemyPtr(OtherEnemy);
	if (!IsValid(OtherEnemy)
		|| OtherEnemy == this
		|| IgnoredEnemiesWhileFalling.Contains(OtherEnemyPtr))
	{
		return;
	}

	IgnoredEnemiesWhileFalling.Add(OtherEnemyPtr);
	GetCapsuleComponent()->IgnoreActorWhenMoving(OtherEnemy, true);
	OtherEnemy->GetCapsuleComponent()->IgnoreActorWhenMoving(this, true);
}

void AEnemyCharacterBase::BounceOffPlayer(
	APlayerCharacterBase* LandedPlayer
)
{
	if (!IsValid(LandedPlayer))
	{
		return;
	}

	IgnoredPlayerWhileFalling = LandedPlayer;
	GetCapsuleComponent()->IgnoreActorWhenMoving(LandedPlayer, true);
	LandedPlayer->GetCapsuleComponent()->IgnoreActorWhenMoving(this, true);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	float BounceDirection = FMath::Sign(
		GetActorLocation().X - LandedPlayer->GetActorLocation().X
	);
	if (FMath::IsNearlyZero(BounceDirection))
	{
		BounceDirection = FMath::Sign(MovementComponent->Velocity.X);
	}
	if (FMath::IsNearlyZero(BounceDirection))
	{
		BounceDirection = 1.0f;
	}

	const FVector BounceVelocity(
		BounceDirection * FMath::Max(PlayerBounceHorizontalSpeed, 0.0f),
		0.0f,
		-FMath::Max(PlayerBounceDownwardSpeed, 0.0f)
	);
	LaunchCharacter(BounceVelocity, true, true);
}

void AEnemyCharacterBase::RestoreEnemyCollisionAfterLanding()
{
	for (const TWeakObjectPtr<AEnemyCharacterBase>& IgnoredEnemy
		: IgnoredEnemiesWhileFalling)
	{
		if (!IgnoredEnemy.IsValid())
		{
			continue;
		}

		GetCapsuleComponent()->IgnoreActorWhenMoving(
			IgnoredEnemy.Get(),
			false
		);
		IgnoredEnemy->GetCapsuleComponent()->IgnoreActorWhenMoving(
			this,
			false
		);
	}

	IgnoredEnemiesWhileFalling.Reset();

	if (IgnoredPlayerWhileFalling.IsValid())
	{
		APlayerCharacterBase* IgnoredPlayer =
			IgnoredPlayerWhileFalling.Get();
		if (!IgnoredPlayer->IsDead())
		{
			GetCapsuleComponent()->IgnoreActorWhenMoving(
				IgnoredPlayer,
				false
			);
			IgnoredPlayer->GetCapsuleComponent()->IgnoreActorWhenMoving(
				this,
				false
			);
		}
	}

	IgnoredPlayerWhileFalling.Reset();
}
