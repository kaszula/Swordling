#include "Characters/Enemy/EnemyCharacterBase.h"

#include "Characters/Player/PlayerCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DiplomaGame.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(
		TEXT("HealthComponent")
	);

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	GetCapsuleComponent()->SetCollisionProfileName(
		UCollisionProfile::Pawn_ProfileName
	);

	GetMesh()->SetCollisionProfileName(
		UCollisionProfile::NoCollision_ProfileName
	);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintAxisSetting(
		EPlaneConstraintAxisSetting::Y
	);
	MovementComponent->bSnapToPlaneAtStart = true;
	MovementComponent->bOrientRotationToMovement = false;
	MovementComponent->bRunPhysicsWithNoController = true;
	MovementComponent->AirControl = 0.6f;
	JumpMaxCount = 1;

	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	BehaviorActivationTime = GetWorld()->GetTimeSeconds()
		+ InitialBehaviorDelay;

	if (EnemyAnimationClass)
	{
		GetMesh()->SetAnimInstanceClass(EnemyAnimationClass);
	}

	TInlineComponentArray<UStaticMeshComponent*> DecorativeMeshes;
	GetComponents(DecorativeMeshes);
	for (UStaticMeshComponent* DecorativeMesh : DecorativeMeshes)
	{
		if (IsValid(DecorativeMesh))
		{
			DecorativeMesh->SetCollisionProfileName(
				UCollisionProfile::NoCollision_ProfileName
			);
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	PatrolHomeLocation = GetActorLocation();
	ScheduleNextPatrolDecision(true);

	InitialMeshRelativeLocation = GetMesh()->GetRelativeLocation();
	InitialMeshWorldYaw = GetMesh()->GetComponentRotation().Yaw;
	MeshFacingYawOffset = FMath::FindDeltaAngleDegrees(
		GetActorRotation().Yaw,
		InitialMeshWorldYaw
	);
	TargetMeshYaw = InitialMeshWorldYaw;

	PlayerCharacter = Cast<APlayerCharacterBase>(
		UGameplayStatics::GetPlayerCharacter(this, 0)
	);

	UpdatePlayerDetection();

	GetWorldTimerManager().SetTimer(
		DetectionTimerHandle,
		this,
		&AEnemyCharacterBase::UpdatePlayerDetection,
		DetectionCheckInterval,
		true
	);
	UpdateSimulationRelevance(true);

	if (HealthComponent)
	{
		HealthComponent->OnDamaged.AddUObject(
			this,
			&AEnemyCharacterBase::HandleDamaged
		);
		HealthComponent->OnDeath.AddUObject(
			this,
			&AEnemyCharacterBase::HandleDeath
		);

		UE_LOG(
			LogDiplomaGameCombat,
			Display,
			TEXT("%s started with %.1f / %.1f health"),
			*GetName(),
			HealthComponent->GetCurrentHealth(),
			HealthComponent->GetMaxHealth()
		);
	}
}

void AEnemyCharacterBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDead)
	{
		UpdateSimulationRelevance();
	}
	if (!bIsSimulationRelevant)
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!bIsDead
		&& MovementComponent
		&& MovementComponent->IsMovingOnGround()
		&& !HasGroundSupportBelowCenter())
	{
		MovementComponent->SetMovementMode(MOVE_Falling);
	}

	if (bIsDead || bIsStunned || bIsBlocking)
	{
		return;
	}

	if (GetWorld()->GetTimeSeconds() < BehaviorActivationTime)
	{
		return;
	}

	if (bIsPlayerDetected)
	{
		UpdateJump();
		UpdateChaseMovement();
	}
	else
	{
		UpdatePatrol();
	}
	UpdateMeshRotation(DeltaTime);
	UpdateAttack();
}

UHealthComponent* AEnemyCharacterBase::GetHealthComponent() const
{
	return HealthComponent;
}
