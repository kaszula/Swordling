#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "EnemyCharacterBase.generated.h"

class APlayerCharacterBase;
class UAnimInstance;
class UAnimMontage;
class UHealthComponent;
class UMaterialInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPlayerDetectionChanged,
	bool,
	bIsDetected
);

enum class EEnemyPatrolState : uint8
{
	Waiting,
	WalkingOut,
	ReturningHome
};

UCLASS(Blueprintable)
class DIPLOMAGAME_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();

	UFUNCTION(BlueprintPure, Category = "Health")
	UHealthComponent* GetHealthComponent() const;

	UFUNCTION(BlueprintPure, Category = "Detection")
	bool IsPlayerDetected() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformAttackHit();

	bool TryBlockPlayerGroundAttack();
	bool TryApplyStun(float StunChance);
	bool IsAttackingActor(const AActor* TargetActor) const;

	UFUNCTION(BlueprintPure, Category = "Combat|Block")
	bool IsBlocking() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Stun")
	bool IsStunned() const;

	UPROPERTY(BlueprintAssignable, Category = "Detection")
	FOnPlayerDetectionChanged OnPlayerDetectionChanged;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;

	void HandleDeath();
	void FinishDeath();
	bool HasGroundSupportBelowCenter() const;
	void BeginPassingThroughEnemy(AEnemyCharacterBase* OtherEnemy);
	void BounceOffPlayer(APlayerCharacterBase* LandedPlayer);
	void RestoreEnemyCollisionAfterLanding();
	void UpdatePlayerDetection();
	void SetPlayerDetectionState(bool bNewIsPlayerDetected);
	void UpdateSimulationRelevance(bool bUseActivationRange = false);
	void SetSimulationRelevant(bool bNewIsSimulationRelevant);
	void UpdatePatrol();
	void StartPatrol();
	void StartPatrolReturn();
	void FinishPatrol();
	void ScheduleNextPatrolDecision(bool bUseRandomInitialDelay = false);
	bool IsPatrolPathClear(float MovementDirection) const;
	void UpdateJump();
	void UpdateChaseMovement();
	bool IsEnemyBlockingChasePath(float MovementDirection) const;
	void UpdateMeshFacing(float MovementDirection);
	void UpdateMeshRotation(float DeltaTime);
	void ApplyMeshFacingTransform(float MeshYaw);
	void StopChasing();
	void StartStun();
	void EndStun();
	void HandleDamaged();
	void StopHitReact();
	void ClearHitFlash();
	void HandleBlockMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void UpdateAttack();
	void PerformAttack();
	bool IsPlayerInAttackRange() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;
	bool bDeathPresentationStarted = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health|Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Health",
		meta = (ClampMin = "0.0", Units = "s")
	)
	float DeathDisappearDelay = 1.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Health",
		meta = (ClampMin = "0.0", Units = "s")
	)
	float DeathLandingTimeout = 2.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat|Stun")
	bool bIsStunned = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance|Animation")
	TSubclassOf<UAnimInstance> EnemyAnimationClass;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Appearance|Animation",
		meta = (ClampMin = "0.0", Units = "deg/s")
	)
	float MeshTurnSpeed = 1080.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Detection",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float DetectionRange = 500.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Detection",
		meta = (ClampMin = "0.01", Units = "s")
	)
	float DetectionCheckInterval = 0.1f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Detection")
	TObjectPtr<APlayerCharacterBase> PlayerCharacter;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Detection")
	bool bIsPlayerDetected = false;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Behavior",
		meta = (ClampMin = "0.0", Units = "s")
	)
	float InitialBehaviorDelay = 2.1f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Behavior|Optimization",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float SimulationActivationRange = 1800.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Behavior|Optimization",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float SimulationDeactivationRange = 2200.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Behavior|Optimization",
		meta = (ClampMin = "0.1", Units = "s")
	)
	float InactiveTickInterval = 0.5f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Movement",
		meta = (ClampMin = "0.0", Units = "cm/s")
	)
	float ChaseSpeed = 200.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Movement|Crowd",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float EnemyQueueSpacing = 120.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Patrol",
		meta = (ClampMin = "0.0", ClampMax = "1.0")
	)
	float PatrolChance = 0.7f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Patrol",
		meta = (ClampMin = "0.1", Units = "s")
	)
	float PatrolDecisionInterval = 2.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Patrol",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float MinimumPatrolDistance = 150.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Patrol",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float MaximumPatrolDistance = 450.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Patrol",
		meta = (ClampMin = "0.0", Units = "cm/s")
	)
	float PatrolSpeed = 100.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Patrol",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float PatrolObstacleCheckDistance = 50.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Movement|Jump",
		meta = (ClampMin = "0.01", Units = "s")
	)
	float JumpCooldown = 0.75f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Movement|Jump",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float JumpObstacleCheckDistance = 60.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Movement|Jump",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float MaximumJumpDownHeight = 150.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Movement|Landing",
		meta = (ClampMin = "0.0", Units = "cm/s")
	)
	float PlayerBounceHorizontalSpeed = 500.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Movement|Landing",
		meta = (ClampMin = "0.0", Units = "cm/s")
	)
	float PlayerBounceDownwardSpeed = 400.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Combat",
		meta = (ClampMin = "0.0", Units = "cm")
	)
	float AttackRange = 100.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Combat",
		meta = (ClampMin = "0.0")
	)
	float AttackDamage = 20.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Combat|Stun",
		meta = (ClampMin = "0.0", ClampMax = "1.0")
	)
	float BasicAttackStunChance = 0.3f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Combat",
		meta = (ClampMin = "0.01", Units = "s")
	)
	float AttackCooldown = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> StrongAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Block")
	TObjectPtr<UAnimMontage> BlockMontage;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Combat|Block",
		meta = (ClampMin = "0.0", ClampMax = "1.0")
	)
	float BlockChance = 0.25f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat|Block")
	bool bIsBlocking = false;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Combat|Strong Attack",
		meta = (ClampMin = "0.0", ClampMax = "1.0")
	)
	float StrongAttackChance = 0.2f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Combat|Strong Attack",
		meta = (ClampMin = "0.0")
	)
	float StrongAttackDamageMultiplier = 2.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Combat|Strong Attack",
		meta = (ClampMin = "0.0", ClampMax = "1.0")
	)
	float StrongAttackStunChance = 0.9f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Stun")
	TObjectPtr<UAnimMontage> StunMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Hit Feedback")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Hit Feedback")
	TObjectPtr<UMaterialInterface> HitFlashMaterial;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat|Hit Feedback",
		meta = (ClampMin = "0.01", Units = "s")
	)
	float HitReactDuration = 0.3f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat|Hit Feedback",
		meta = (ClampMin = "0.01", Units = "s")
	)
	float HitFlashDuration = 0.08f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat|Stun",
		meta = (ClampMin = "0.01", Units = "s")
	)
	float StunDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Debug")
	bool bDrawAttackDebug = false;

	float MeshFacingYawOffset = 0.0f;
	float InitialMeshWorldYaw = 0.0f;
	float TargetMeshYaw = 0.0f;
	float MeshTurnDirection = 0.0f;
	FVector InitialMeshRelativeLocation = FVector::ZeroVector;
	float NextAttackTime = 0.0f;
	float NextJumpTime = 0.0f;
	float BehaviorActivationTime = 0.0f;
	bool bIsSimulationRelevant = true;
	float CurrentAttackDamageMultiplier = 1.0f;
	float CurrentAttackStunChance = 0.0f;
	TArray<TWeakObjectPtr<AEnemyCharacterBase>> IgnoredEnemiesWhileFalling;
	TWeakObjectPtr<APlayerCharacterBase> IgnoredPlayerWhileFalling;
	EEnemyPatrolState PatrolState = EEnemyPatrolState::Waiting;
	FVector PatrolHomeLocation = FVector::ZeroVector;
	float PatrolTargetX = 0.0f;
	float NextPatrolDecisionTime = 0.0f;

	FTimerHandle DetectionTimerHandle;
	FTimerHandle StunTimerHandle;
	FTimerHandle HitReactTimerHandle;
	FTimerHandle HitFlashTimerHandle;
	FTimerHandle DeathLandingTimerHandle;
};
