#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Characters/Player/PlayerCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEnemyCharacterBase::UpdateJump()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	UWorld* World = GetWorld();
	if (bIsDead
		|| bIsStunned
		|| !bIsPlayerDetected
		|| !IsValid(PlayerCharacter)
		|| PlayerCharacter->IsDead()
		|| PlayerCharacter->GetCharacterMovement()->IsFalling()
		|| !MovementComponent
		|| MovementComponent->IsFalling()
		|| IsAttackingActor(PlayerCharacter)
		|| IsPlayerInAttackRange()
		|| !World
		|| World->GetTimeSeconds() < NextJumpTime
		|| !CanJump())
	{
		return;
	}

	const FVector ActorLocation = GetActorLocation();
	const FVector DirectionToPlayer =
		PlayerCharacter->GetActorLocation() - ActorLocation;
	const float HorizontalDistance = FMath::Abs(DirectionToPlayer.X);
	if (FMath::IsNearlyZero(HorizontalDistance))
	{
		return;
	}

	const float Gravity = FMath::Abs(MovementComponent->GetGravityZ());
	const float JumpVelocity = MovementComponent->JumpZVelocity;
	if (Gravity <= UE_SMALL_NUMBER || JumpVelocity <= 0.0f)
	{
		return;
	}

	const float MaximumJumpHeight =
		FMath::Square(JumpVelocity) / (2.0f * Gravity);
	const float HeightDifference = DirectionToPlayer.Z;
	const float ReachDiscriminant = FMath::Square(JumpVelocity)
		- 2.0f * Gravity * HeightDifference;
	const bool bPlayerHeightIsReachable =
		ReachDiscriminant >= 0.0f
		&& HeightDifference <= MaximumJumpHeight;
	const float FlightTime = bPlayerHeightIsReachable
		? (JumpVelocity + FMath::Sqrt(ReachDiscriminant)) / Gravity
		: 0.0f;
	const float HorizontalJumpReach =
		MovementComponent->GetMaxSpeed() * FlightTime;
	const bool bReachablePlayerIsAbove =
		HeightDifference > MovementComponent->MaxStepHeight
		&& HorizontalDistance <= HorizontalJumpReach + AttackRange;

	float CapsuleRadius = 0.0f;
	float CapsuleHalfHeight = 0.0f;
	GetCapsuleComponent()->GetScaledCapsuleSize(
		CapsuleRadius,
		CapsuleHalfHeight
	);

	const float MovementDirection = FMath::Sign(DirectionToPlayer.X);
	if (IsEnemyBlockingChasePath(MovementDirection))
	{
		return;
	}

	const float ProbeDistance =
		CapsuleRadius + FMath::Max(JumpObstacleCheckDistance, 0.0f);
	const float CapsuleBottom = ActorLocation.Z - CapsuleHalfHeight;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(EnemyJump), false);
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(PlayerCharacter);

	const FVector LowProbeStart(
		ActorLocation.X,
		ActorLocation.Y,
		CapsuleBottom + MovementComponent->MaxStepHeight + 5.0f
	);
	const FVector LowProbeEnd = LowProbeStart
		+ FVector::ForwardVector * MovementDirection * ProbeDistance;
	FHitResult LowObstacleHit;
	const bool bObstacleAhead = World->LineTraceSingleByChannel(
		LowObstacleHit,
		LowProbeStart,
		LowProbeEnd,
		ECC_WorldStatic,
		QueryParams
	);

	const FVector HighProbeStart(
		ActorLocation.X,
		ActorLocation.Y,
		CapsuleBottom + MaximumJumpHeight
	);
	const FVector HighProbeEnd = HighProbeStart
		+ FVector::ForwardVector * MovementDirection * ProbeDistance;
	FHitResult HighObstacleHit;
	const bool bObstacleTooHigh = World->LineTraceSingleByChannel(
		HighObstacleHit,
		HighProbeStart,
		HighProbeEnd,
		ECC_WorldStatic,
		QueryParams
	);
	const bool bLowObstacleAhead = bObstacleAhead && !bObstacleTooHigh;

	const FVector GroundProbeStart(
		LowProbeEnd.X,
		ActorLocation.Y,
		CapsuleBottom + MovementComponent->MaxStepHeight
	);
	const FVector GroundProbeEnd(
		GroundProbeStart.X,
		GroundProbeStart.Y,
		CapsuleBottom - 10.0f
	);
	FHitResult GroundAheadHit;
	const bool bHasGroundAhead = World->LineTraceSingleByChannel(
		GroundAheadHit,
		GroundProbeStart,
		GroundProbeEnd,
		ECC_WorldStatic,
		QueryParams
	);

	bool bReachableLandingAcrossGap = false;
	if (!bHasGroundAhead && HorizontalJumpReach > ProbeDistance)
	{
		const float LandingDistance = FMath::Min(
			HorizontalDistance,
			HorizontalJumpReach * 0.8f
		);
		const float LandingX = ActorLocation.X
			+ MovementDirection * LandingDistance;
		const FVector LandingProbeStart(
			LandingX,
			ActorLocation.Y,
			CapsuleBottom + MaximumJumpHeight
		);
		const FVector LandingProbeEnd(
			LandingX,
			ActorLocation.Y,
			CapsuleBottom - FMath::Max(MaximumJumpDownHeight, 0.0f)
		);
		FHitResult LandingHit;
		bReachableLandingAcrossGap = World->LineTraceSingleByChannel(
			LandingHit,
			LandingProbeStart,
			LandingProbeEnd,
			ECC_WorldStatic,
			QueryParams
		);
	}

	if (!bLowObstacleAhead
		&& !bReachableLandingAcrossGap
		&& !(bReachablePlayerIsAbove && !bObstacleTooHigh))
	{
		return;
	}

	Jump();
	NextJumpTime = World->GetTimeSeconds()
		+ FMath::Max(JumpCooldown, UE_SMALL_NUMBER);
}
