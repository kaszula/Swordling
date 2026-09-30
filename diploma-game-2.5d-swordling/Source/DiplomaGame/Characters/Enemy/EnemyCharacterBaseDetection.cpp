#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Characters/Player/PlayerCharacterBase.h"
#include "DiplomaGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

bool AEnemyCharacterBase::IsPlayerDetected() const
{
	return bIsPlayerDetected;
}

void AEnemyCharacterBase::UpdatePlayerDetection()
{
	if (bIsDead || !bIsSimulationRelevant)
	{
		SetPlayerDetectionState(false);
		return;
	}

	if (!IsValid(PlayerCharacter))
	{
		PlayerCharacter = Cast<APlayerCharacterBase>(
			UGameplayStatics::GetPlayerCharacter(this, 0)
		);
	}

	if (!IsValid(PlayerCharacter) || PlayerCharacter->IsDead())
	{
		SetPlayerDetectionState(false);
		return;
	}

	const float DistanceSquared = FVector::DistSquared(
		GetActorLocation(),
		PlayerCharacter->GetActorLocation()
	);

	SetPlayerDetectionState(
		DistanceSquared <= FMath::Square(DetectionRange)
	);
}

void AEnemyCharacterBase::UpdateSimulationRelevance(
	const bool bUseActivationRange
)
{
	if (!IsValid(PlayerCharacter))
	{
		PlayerCharacter = Cast<APlayerCharacterBase>(
			UGameplayStatics::GetPlayerCharacter(this, 0)
		);
	}
	if (!IsValid(PlayerCharacter))
	{
		SetSimulationRelevant(false);
		return;
	}

	const float RelevantRange = bUseActivationRange
		|| !bIsSimulationRelevant
		? SimulationActivationRange
		: FMath::Max(
			SimulationDeactivationRange,
			SimulationActivationRange
		);
	const bool bShouldBeSimulationRelevant = FVector::DistSquared(
		GetActorLocation(),
		PlayerCharacter->GetActorLocation()
	) <= FMath::Square(FMath::Max(RelevantRange, 0.0f));

	SetSimulationRelevant(bShouldBeSimulationRelevant);
}

void AEnemyCharacterBase::SetSimulationRelevant(
	const bool bNewIsSimulationRelevant
)
{
	if (bIsSimulationRelevant == bNewIsSimulationRelevant)
	{
		return;
	}

	bIsSimulationRelevant = bNewIsSimulationRelevant;
	PrimaryActorTick.TickInterval = bIsSimulationRelevant
		? 0.0f
		: FMath::Max(InactiveTickInterval, 0.1f);
	GetMesh()->bPauseAnims = !bIsSimulationRelevant;

	if (!bIsSimulationRelevant)
	{
		GetWorldTimerManager().ClearTimer(DetectionTimerHandle);
		SetPlayerDetectionState(false);
		StopChasing();
		return;
	}

	GetWorldTimerManager().SetTimer(
		DetectionTimerHandle,
		this,
		&AEnemyCharacterBase::UpdatePlayerDetection,
		DetectionCheckInterval,
		true
	);
	ScheduleNextPatrolDecision(true);
	UpdatePlayerDetection();
}

void AEnemyCharacterBase::SetPlayerDetectionState(
	const bool bNewIsPlayerDetected
)
{
	if (bIsPlayerDetected == bNewIsPlayerDetected)
	{
		return;
	}

	bIsPlayerDetected = bNewIsPlayerDetected;
	OnPlayerDetectionChanged.Broadcast(bIsPlayerDetected);

	if (bIsPlayerDetected)
	{
		PatrolState = EEnemyPatrolState::Waiting;
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
		StopChasing();
	}
	else
	{
		StopChasing();
		PatrolHomeLocation = GetActorLocation();
		FinishPatrol();
	}

	UE_LOG(
		LogDiplomaGameCombat,
		Display,
		TEXT("%s %s player"),
		*GetName(),
		bIsPlayerDetected ? TEXT("detected") : TEXT("lost")
	);
}
