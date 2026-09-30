#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Characters/Player/PlayerCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"

bool AEnemyCharacterBase::HasGroundSupportBelowCenter() const
{
	const UWorld* World = GetWorld();
	const UCapsuleComponent* Capsule = GetCapsuleComponent();
	const UCharacterMovementComponent* MovementComponent =
		GetCharacterMovement();
	if (!World || !Capsule || !MovementComponent)
	{
		return false;
	}

	const float CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	const FVector CapsuleLocation = Capsule->GetComponentLocation();
	const float CapsuleBottom = CapsuleLocation.Z - CapsuleHalfHeight;
	const float ProbeDepth = FMath::Max(
		MovementComponent->MaxStepHeight,
		10.0f
	);
	const FVector ProbeStart(
		CapsuleLocation.X,
		CapsuleLocation.Y,
		CapsuleBottom + 5.0f
	);
	const FVector ProbeEnd(
		CapsuleLocation.X,
		CapsuleLocation.Y,
		CapsuleBottom - ProbeDepth
	);

	FCollisionQueryParams QueryParams(
		SCENE_QUERY_STAT(EnemyCenterGroundSupport),
		false
	);
	QueryParams.AddIgnoredActor(this);

	FHitResult GroundHit;
	return World->LineTraceSingleByChannel(
		GroundHit,
		ProbeStart,
		ProbeEnd,
		ECC_WorldStatic,
		QueryParams
	) && MovementComponent->IsWalkable(GroundHit);
}

void AEnemyCharacterBase::UpdateChaseMovement()
{
	if (bIsDead
		|| !bIsPlayerDetected
		|| !IsValid(PlayerCharacter)
		|| PlayerCharacter->IsDead())
	{
		StopChasing();
		return;
	}

	const FVector DirectionToPlayer =
		PlayerCharacter->GetActorLocation() - GetActorLocation();

	if (FMath::IsNearlyZero(DirectionToPlayer.X))
	{
		StopChasing();
		return;
	}

	const float MovementDirection = FMath::Sign(DirectionToPlayer.X);
	UpdateMeshFacing(MovementDirection);

	if (DirectionToPlayer.SizeSquared() <= FMath::Square(AttackRange))
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			StopChasing();
		}
		return;
	}

	if (IsEnemyBlockingChasePath(MovementDirection))
	{
		StopChasing();
		return;
	}

	AddMovementInput(FVector::ForwardVector, MovementDirection);
}

bool AEnemyCharacterBase::IsEnemyBlockingChasePath(
	const float MovementDirection
) const
{
	UWorld* const World = GetWorld();
	if (!World
		|| !IsValid(PlayerCharacter)
		|| FMath::IsNearlyZero(MovementDirection))
	{
		return false;
	}

	const FVector ActorLocation = GetActorLocation();
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const float DistanceToPlayerX = FMath::Abs(
		PlayerLocation.X - ActorLocation.X
	);
	const UCharacterMovementComponent* MovementComponent =
		GetCharacterMovement();
	const float VerticalTolerance = MovementComponent
		? MovementComponent->MaxStepHeight + 5.0f
		: 0.0f;
	const float QueueSpacing = FMath::Max(EnemyQueueSpacing, 0.0f);

	for (TActorIterator<AEnemyCharacterBase> EnemyIterator(World);
		EnemyIterator;
		++EnemyIterator)
	{
		const AEnemyCharacterBase* OtherEnemy = *EnemyIterator;
		if (!IsValid(OtherEnemy)
			|| OtherEnemy == this
			|| OtherEnemy->bIsDead)
		{
			continue;
		}

		const FVector OtherLocation = OtherEnemy->GetActorLocation();
		const float OtherDirectionToPlayer = FMath::Sign(
			PlayerLocation.X - OtherLocation.X
		);
		if (!FMath::IsNearlyEqual(
			OtherDirectionToPlayer,
			MovementDirection
		))
		{
			continue;
		}

		const float DistanceToOtherX = FMath::Abs(
			OtherLocation.X - ActorLocation.X
		);
		if (DistanceToOtherX > QueueSpacing
			|| FMath::Abs(OtherLocation.Z - ActorLocation.Z)
				> VerticalTolerance)
		{
			continue;
		}

		const float OtherDistanceToPlayerX = FMath::Abs(
			PlayerLocation.X - OtherLocation.X
		);
		if (OtherDistanceToPlayerX < DistanceToPlayerX)
		{
			return true;
		}
	}

	return false;
}

void AEnemyCharacterBase::UpdateMeshFacing(const float MovementDirection)
{
	if (FMath::IsNearlyZero(MovementDirection))
	{
		return;
	}

	const float MovementYaw = MovementDirection > 0.0f ? 0.0f : 180.0f;
	TargetMeshYaw = MovementYaw + MeshFacingYawOffset;

	FRotator MeshRotation = GetMesh()->GetComponentRotation();
	if (MeshTurnSpeed <= 0.0f)
	{
		MeshRotation.Yaw = TargetMeshYaw;
		ApplyMeshFacingTransform(MeshRotation.Yaw);
		return;
	}

	if (FMath::IsNearlyZero(
		FMath::FindDeltaAngleDegrees(MeshRotation.Yaw, TargetMeshYaw),
		0.1f
	))
	{
		return;
	}

	const float TurnDelta = FMath::FindDeltaAngleDegrees(
		MeshRotation.Yaw,
		TargetMeshYaw
	);

	if (FMath::IsNearlyEqual(FMath::Abs(TurnDelta), 180.0f, 0.1f))
	{
		const float CurrentYaw = FRotator::ClampAxis(MeshRotation.Yaw);
		const float DesiredYaw = FRotator::ClampAxis(TargetMeshYaw);
		MeshTurnDirection = CurrentYaw > DesiredYaw ? -1.0f : 1.0f;
	}
	else
	{
		MeshTurnDirection = FMath::Sign(TurnDelta);
	}
}

void AEnemyCharacterBase::UpdateMeshRotation(const float DeltaTime)
{
	FRotator MeshRotation = GetMesh()->GetComponentRotation();
	const float RemainingTurn = FMath::FindDeltaAngleDegrees(
		MeshRotation.Yaw,
		TargetMeshYaw
	);
	const float MaximumTurn = MeshTurnSpeed * DeltaTime;

	if (FMath::Abs(RemainingTurn) <= MaximumTurn)
	{
		MeshRotation.Yaw = TargetMeshYaw;
	}
	else
	{
		MeshRotation.Yaw = FRotator::NormalizeAxis(
			MeshRotation.Yaw + MeshTurnDirection * MaximumTurn
		);
	}

	ApplyMeshFacingTransform(MeshRotation.Yaw);
}

void AEnemyCharacterBase::ApplyMeshFacingTransform(const float MeshYaw)
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (!IsValid(CharacterMesh))
	{
		return;
	}

	FRotator MeshRotation = CharacterMesh->GetComponentRotation();
	MeshRotation.Yaw = MeshYaw;
	CharacterMesh->SetWorldRotation(MeshRotation);

	const float FacingDeltaYaw = FMath::FindDeltaAngleDegrees(
		InitialMeshWorldYaw,
		MeshYaw
	);
	const FVector RotatedPlanarOffset = FRotator(
		0.0f,
		FacingDeltaYaw,
		0.0f
	).RotateVector(FVector(
		InitialMeshRelativeLocation.X,
		InitialMeshRelativeLocation.Y,
		0.0f
	));

	CharacterMesh->SetRelativeLocation(FVector(
		RotatedPlanarOffset.X,
		RotatedPlanarOffset.Y,
		InitialMeshRelativeLocation.Z
	));
}

void AEnemyCharacterBase::StopChasing()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent->IsFalling())
	{
		FVector FallingVelocity = MovementComponent->Velocity;
		FallingVelocity.X = 0.0f;
		FallingVelocity.Y = 0.0f;
		MovementComponent->Velocity = FallingVelocity;
		return;
	}

	MovementComponent->StopMovementImmediately();
}
