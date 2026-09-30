#include "Characters/Player/PlayerCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/AudioComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

void APlayerCharacterBase::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (!IsValid(CharacterMesh))
	{
		SetActorTickEnabled(false);
		return;
	}

	FRotator MeshRotation = CharacterMesh->GetComponentRotation();
	const float RemainingTurn = FMath::FindDeltaAngleDegrees(
		MeshRotation.Yaw,
		TargetMeshYaw
	);
	const float MaximumTurn = MeshTurnSpeed * DeltaSeconds;

	if (FMath::Abs(RemainingTurn) <= MaximumTurn)
	{
		MeshRotation.Yaw = TargetMeshYaw;
		SetActorTickEnabled(false);
	}
	else
	{
		MeshRotation.Yaw = FRotator::NormalizeAxis(
			MeshRotation.Yaw + MeshTurnDirection * MaximumTurn
		);
	}

	ApplyMeshFacingTransform(MeshRotation.Yaw);
}

void APlayerCharacterBase::Move(const FInputActionValue& Value)
{
	if (bIsDead || bIsRespawning || bIsStunned)
	{
		StopRunningSound();
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (StrongAttackMontage
		&& AnimInstance
		&& !GetCharacterMovement()->IsFalling()
		&& AnimInstance->Montage_IsPlaying(StrongAttackMontage))
	{
		StopRunningSound();
		return;
	}

	const FVector2D MovementValue = Value.Get<FVector2D>();
	UpdateRunningSound(MovementValue.X);

	UpdateMeshFacing(MovementValue.X);
	AddMovementInput(FVector::ForwardVector, MovementValue.X);
}

void APlayerCharacterBase::StopMove(const FInputActionValue&)
{
	StopRunningSound();
}

void APlayerCharacterBase::UpdateRunningSound(
	const float MovementDirection
)
{
	if (!RunningSound
		|| FMath::IsNearlyZero(MovementDirection)
		|| GetCharacterMovement()->IsFalling()
		|| !GetMesh())
	{
		StopRunningSound();
		return;
	}

	if (!IsValid(RunningAudioComponent)
		|| !RunningAudioComponent->IsPlaying())
	{
		RunningAudioComponent = UGameplayStatics::SpawnSoundAttached(
			RunningSound,
			GetMesh()
		);
	}
}

void APlayerCharacterBase::StopRunningSound()
{
	if (IsValid(RunningAudioComponent))
	{
		RunningAudioComponent->Stop();
	}
	RunningAudioComponent = nullptr;
}

void APlayerCharacterBase::StartJump()
{
	if (bIsDead || bIsRespawning || bIsStunned)
	{
		return;
	}

	StopRunningSound();

	const bool bIsStartingDoubleJump =
		GetCharacterMovement()->IsFalling()
		&& JumpCurrentCount > 0
		&& JumpCurrentCount < JumpMaxCount;

	if (bIsStartingDoubleJump)
	{
		bIsDoubleJumpSpinActive = true;

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Stop(0.0f, AttackMontage);
			AnimInstance->Montage_Stop(0.0f, StrongAttackMontage);
		}
	}

	Jump();
}

void APlayerCharacterBase::StopJump()
{
	StopJumping();
}

void APlayerCharacterBase::Landed(const FHitResult& Hit)
{
	const float ImpactSpeed = FMath::Max(-GetVelocity().Z, 0.0f);

	Super::Landed(Hit);
	bIsDoubleJumpSpinActive = false;

	if (!bIsDead && IsValid(HealthComponent))
	{
		HealthComponent->ApplyDamage(CalculateFallDamage(ImpactSpeed));
	}

	if (bIsDead)
	{
		FinishDeath();
	}
}

float APlayerCharacterBase::CalculateFallDamage(const float ImpactSpeed) const
{
	if (ImpactSpeed >= FatalFallImpactSpeed)
	{
		return IsValid(HealthComponent)
			? HealthComponent->GetMaxHealth()
			: 0.0f;
	}

	if (ImpactSpeed >= HeavyFallImpactSpeed)
	{
		return FMath::Max(HeavyFallDamage, 0.0f);
	}

	if (ImpactSpeed >= MediumFallImpactSpeed)
	{
		return FMath::Max(MediumFallDamage, 0.0f);
	}

	if (ImpactSpeed >= LightFallImpactSpeed)
	{
		return FMath::Max(LightFallDamage, 0.0f);
	}

	return 0.0f;
}

void APlayerCharacterBase::UpdateMeshFacing(const float MovementDirection)
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
		SetActorTickEnabled(false);
		return;
	}

	if (!FMath::IsNearlyZero(
		FMath::FindDeltaAngleDegrees(MeshRotation.Yaw, TargetMeshYaw),
		0.1f
	))
	{
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

		SetActorTickEnabled(true);
	}
}

void APlayerCharacterBase::ApplyMeshFacingTransform(const float MeshYaw)
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
