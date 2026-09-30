#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEnemyCharacterBase::UpdatePatrol()
{
	UWorld* World = GetWorld();
	if (bIsDead
		|| bIsStunned
		|| bIsPlayerDetected
		|| GetCharacterMovement()->IsFalling()
		|| !World)
	{
		return;
	}

	if (PatrolState == EEnemyPatrolState::Waiting)
	{
		if (World->GetTimeSeconds() < NextPatrolDecisionTime)
		{
			return;
		}

		if (FMath::FRand() < FMath::Clamp(PatrolChance, 0.0f, 1.0f))
		{
			StartPatrol();
		}
		else
		{
			ScheduleNextPatrolDecision();
		}
		return;
	}

	const float DistanceToTarget = PatrolTargetX - GetActorLocation().X;
	if (FMath::Abs(DistanceToTarget) <= 5.0f)
	{
		if (PatrolState == EEnemyPatrolState::WalkingOut)
		{
			StartPatrolReturn();
		}
		else
		{
			FinishPatrol();
		}
		return;
	}

	const float MovementDirection = FMath::Sign(DistanceToTarget);
	if (!IsPatrolPathClear(MovementDirection))
	{
		if (PatrolState == EEnemyPatrolState::WalkingOut)
		{
			StartPatrolReturn();
		}
		else
		{
			FinishPatrol();
		}
		return;
	}

	UpdateMeshFacing(MovementDirection);
	AddMovementInput(FVector::ForwardVector, MovementDirection);
}

void AEnemyCharacterBase::StartPatrol()
{
	const float PatrolDirection = FMath::FRand() < 0.5f ? -1.0f : 1.0f;
	const float MinimumDistance = FMath::Max(
		MinimumPatrolDistance,
		0.0f
	);
	const float MaximumDistance = FMath::Max(
		MaximumPatrolDistance,
		MinimumDistance
	);
	const float SelectedDistance = FMath::FRandRange(
		MinimumDistance,
		MaximumDistance
	);
	PatrolTargetX = PatrolHomeLocation.X
		+ PatrolDirection * SelectedDistance;
	PatrolState = EEnemyPatrolState::WalkingOut;
	GetCharacterMovement()->MaxWalkSpeed = FMath::Max(PatrolSpeed, 0.0f);

	if (FMath::IsNearlyZero(SelectedDistance)
		|| FMath::IsNearlyZero(PatrolSpeed)
		|| !IsPatrolPathClear(PatrolDirection))
	{
		FinishPatrol();
	}
}

void AEnemyCharacterBase::StartPatrolReturn()
{
	PatrolTargetX = PatrolHomeLocation.X;
	PatrolState = EEnemyPatrolState::ReturningHome;
	GetCharacterMovement()->MaxWalkSpeed = FMath::Max(PatrolSpeed, 0.0f);

	if (FMath::IsNearlyEqual(GetActorLocation().X, PatrolTargetX, 5.0f))
	{
		FinishPatrol();
	}
}

void AEnemyCharacterBase::FinishPatrol()
{
	PatrolState = EEnemyPatrolState::Waiting;
	StopChasing();
	ScheduleNextPatrolDecision();
}

void AEnemyCharacterBase::ScheduleNextPatrolDecision(
	const bool bUseRandomInitialDelay
)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float DecisionInterval = FMath::Max(
		PatrolDecisionInterval,
		0.1f
	);
	const float Delay = bUseRandomInitialDelay
		? FMath::FRandRange(0.0f, DecisionInterval)
		: DecisionInterval;
	NextPatrolDecisionTime = World->GetTimeSeconds() + Delay;
}

bool AEnemyCharacterBase::IsPatrolPathClear(
	const float MovementDirection
) const
{
	const UWorld* World = GetWorld();
	const UCapsuleComponent* EnemyCapsule = GetCapsuleComponent();
	const UCharacterMovementComponent* MovementComponent =
		GetCharacterMovement();
	if (!World
		|| !EnemyCapsule
		|| !MovementComponent
		|| FMath::IsNearlyZero(MovementDirection))
	{
		return false;
	}

	float CapsuleRadius = 0.0f;
	float CapsuleHalfHeight = 0.0f;
	EnemyCapsule->GetScaledCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	const FVector ActorLocation = GetActorLocation();
	const float CapsuleBottom = ActorLocation.Z - CapsuleHalfHeight;
	const float ProbeDistance = CapsuleRadius
		+ FMath::Max(PatrolObstacleCheckDistance, 0.0f);
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(EnemyPatrol), false);
	QueryParams.AddIgnoredActor(this);

	const FVector ObstacleProbeStart(
		ActorLocation.X,
		ActorLocation.Y,
		CapsuleBottom + MovementComponent->MaxStepHeight + 5.0f
	);
	const FVector ObstacleProbeEnd = ObstacleProbeStart
		+ FVector::ForwardVector * MovementDirection * ProbeDistance;
	FHitResult ObstacleHit;
	if (World->LineTraceSingleByChannel(
		ObstacleHit,
		ObstacleProbeStart,
		ObstacleProbeEnd,
		ECC_WorldStatic,
		QueryParams
	))
	{
		return false;
	}

	const FVector GroundProbeStart(
		ObstacleProbeEnd.X,
		ActorLocation.Y,
		CapsuleBottom + MovementComponent->MaxStepHeight
	);
	const FVector GroundProbeEnd(
		GroundProbeStart.X,
		GroundProbeStart.Y,
		CapsuleBottom - 10.0f
	);
	FHitResult GroundHit;
	return World->LineTraceSingleByChannel(
		GroundHit,
		GroundProbeStart,
		GroundProbeEnd,
		ECC_WorldStatic,
		QueryParams
	);
}
