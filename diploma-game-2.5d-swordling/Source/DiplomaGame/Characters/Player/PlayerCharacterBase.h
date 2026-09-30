#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "PlayerCharacterBase.generated.h"

class UInputAction;
class UInputMappingContext;
class UHealthComponent;
class UStaminaComponent;
class UTitleRevealWidget;
class UUserWidget;
class UAudioComponent;
class UAnimInstance;
class UAnimMontage;
class UMaterialInterface;
class USoundBase;
class USkeletalMesh;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerAvatar : uint8
{
	TinyHeroBoy UMETA(DisplayName = "Tiny Hero Boy"),
	TinyHeroGirl UMETA(DisplayName = "Tiny Hero Girl")
};

USTRUCT(BlueprintType)
struct FPlayerAvatarStats
{
	GENERATED_BODY()

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Avatar Stats",
		meta = (ClampMin = "1")
	)
	int32 MaxLives = 1;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Avatar Stats",
		meta = (ClampMin = "0.0")
	)
	float HealthMultiplier = 1.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Avatar Stats",
		meta = (ClampMin = "0.0")
	)
	float StaminaMultiplier = 1.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Avatar Stats",
		meta = (ClampMin = "0.0")
	)
	float AttackMultiplier = 1.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPlayerLivesChangedSignature,
	int32,
	RemainingLives
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerGameOverSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameCompletedSignature);

UCLASS(Blueprintable)
class DIPLOMAGAME_API APlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacterBase();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetPlayerAvatar(EPlayerAvatar NewAvatar);

	UFUNCTION(BlueprintPure, Category = "Appearance")
	EPlayerAvatar GetPlayerAvatar() const;
	void RecordFangKill();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformAttackHit();

	bool TryParryEnemyBasicAttack();
	bool TryApplyStun(float StunChance);
	bool IsAttackingActor(const AActor* TargetActor) const;
	void PlayWeaponClashSound();

	UFUNCTION(BlueprintPure, Category = "Health")
	UHealthComponent* GetHealthComponent() const;

	UFUNCTION(BlueprintPure, Category = "Stamina")
	UStaminaComponent* GetStaminaComponent() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const;

	UFUNCTION(BlueprintPure, Category = "Health|Lives")
	int32 GetRemainingLives() const;

	UFUNCTION(BlueprintPure, Category = "Health|Lives")
	int32 GetMaxLives() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsRespawning() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsGameOver() const;

	UFUNCTION(BlueprintCallable, Category = "UI|Title Screen")
	bool StartTitleReveal();
	void ApplyTitleRevealMovement(float MovementScale);
	void StopTitleRevealMovement();
	void FinishTitleReveal();

	UFUNCTION(BlueprintCallable, Category = "Game Completion")
	bool StartGameCompletion(
		TSubclassOf<UTitleRevealWidget> EndRevealWidgetClass,
		TSubclassOf<UUserWidget> ScoreboardWidgetClass
	);

	UFUNCTION(BlueprintPure, Category = "Game Completion")
	bool IsGameCompleted() const;

	void FinishGameCompletion();

	UPROPERTY(BlueprintAssignable, Category = "HUD|Lives")
	FOnPlayerLivesChangedSignature OnLivesChanged;

	UPROPERTY(BlueprintAssignable, Category = "HUD|Lives")
	FOnPlayerLivesChangedSignature OnLifeLossStarted;

	UPROPERTY(BlueprintAssignable, Category = "HUD|Lives")
	FOnPlayerLivesChangedSignature OnRespawnCompleted;

	UPROPERTY(BlueprintAssignable, Category = "HUD|Game Over")
	FOnPlayerGameOverSignature OnGameOver;

	UPROPERTY(BlueprintAssignable, Category = "Game Completion")
	FOnGameCompletedSignature OnGameCompleted;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void ApplySelectedAvatar();
	void ApplySelectedAvatarStats();
	void CacheBaseAvatarStats();
	const FPlayerAvatarStats& GetSelectedAvatarStats() const;
	FString GetEnteredPlayerName() const;
	FString GetSelectedHeroName() const;
	void ShowTitleScreen();
	void PlayMenuMusic(bool bRestartFromBeginning);
	void StopMenuMusic();
	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	void UpdateRunningSound(float MovementDirection);
	void StopRunningSound();
	void StartJump();
	void StopJump();
	float CalculateFallDamage(float ImpactSpeed) const;
	void Attack();
	void StrongAttack();
	void StartParry();
	void EndParryWindow();
	void HandleParryWindowExpired();
	void StartAttack(
		UAnimMontage* Montage,
		float DamageMultiplier,
		float StunChance,
		float StaminaCost
	);
	void UpdateMeshFacing(float MovementDirection);
	void ApplyMeshFacingTransform(float MeshYaw);
	void HandleDeath();
	void FinishDeath();
	void SetEnemiesIgnoredDuringDeath(bool bShouldIgnore);
	void RespawnAtDeathLocation();
	void StartRespawnAnimation();
	void HandleRespawnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void CompleteRespawn();
	void BeginGameOver();
	void StartStun();
	void EndStun();
	void HandleDamaged();
	void HandleDamageApplied(float DamageAmount);
	void StopHitReact();
	void ClearHitFlash();

	float MeshFacingYawOffset = 0.0f;
	float InitialMeshWorldYaw = 0.0f;
	float TargetMeshYaw = 0.0f;
	float MeshTurnDirection = 0.0f;
	FVector InitialMeshRelativeLocation = FVector::ZeroVector;
	float NextAttackTime = 0.0f;
	float CurrentAttackDamageMultiplier = 1.0f;
	float CurrentAttackStunChance = 0.0f;
	float LastWeaponClashSoundTime = -1.0f;
	bool bCurrentAttackStartedInAir = false;

	UPROPERTY(BlueprintReadWrite, Category = "Combat|Animation")
	bool bIsDoubleJumpSpinActive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	EPlayerAvatar SelectedAvatar = EPlayerAvatar::TinyHeroBoy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance|Database")
	FString TinyHeroBoyDatabaseName = TEXT("Tristan");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance|Database")
	FString TinyHeroGirlDatabaseName = TEXT("Willow");

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Appearance|Stats"
	)
	FPlayerAvatarStats TinyHeroBoyStats;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Appearance|Stats"
	)
	FPlayerAvatarStats TinyHeroGirlStats;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Transient,
		Category = "Appearance|Stats"
	)
	float BaseMaxHealth = 0.0f;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Transient,
		Category = "Appearance|Stats"
	)
	float BaseMaxStamina = 0.0f;

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Transient,
		Category = "Appearance|Stats"
	)
	float BaseAttackDamage = 0.0f;

	bool bBaseAvatarStatsCached = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TObjectPtr<USkeletalMesh> TinyHeroBoyMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TObjectPtr<USkeletalMesh> TinyHeroGirlMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance|Animation")
	TSubclassOf<UAnimInstance> PlayerAnimationClass;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Appearance|Animation",
		meta = (ClampMin = "0.0", Units = "deg/s")
	)
	float MeshTurnSpeed = 1080.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Title Screen")
	TSubclassOf<UUserWidget> TitleScreenWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> TitleScreenWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Title Screen")
	TSubclassOf<UTitleRevealWidget> TitleRevealWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UTitleRevealWidget> TitleRevealWidget;

	UPROPERTY(Transient)
	TSubclassOf<UUserWidget> CompletionScoreboardWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> ScoreboardWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> StrongAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ParryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackRange = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackHalfWidth = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackHalfHeight = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackDamage = 25.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Stamina|Costs",
		meta = (ClampMin = "0.0")
	)
	float AttackStaminaCost = 10.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Stamina|Costs",
		meta = (ClampMin = "0.0")
	)
	float StrongAttackStaminaCost = 20.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Stamina|Costs",
		meta = (ClampMin = "0.0")
	)
	float ParryStaminaCost = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> StrongAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Parry")
	TObjectPtr<UAnimMontage> ParryMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Parry")
	TObjectPtr<USoundBase> SuccessfulParrySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Audio")
	TObjectPtr<USoundBase> WeaponClashSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Audio")
	TObjectPtr<USoundBase> RunningSound;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "UI|Audio"
	)
	TObjectPtr<UAudioComponent> MenuMusicAudioComponent;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat|Parry",
		meta = (ClampMin = "0.01", Units = "s")
	)
	float ParryWindowDuration = 0.25f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat|Parry")
	bool bIsParryWindowActive = false;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat",
		meta = (ClampMin = "0.0")
	)
	float StrongAttackDamageMultiplier = 2.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat|Stun",
		meta = (ClampMin = "0.0", ClampMax = "1.0")
	)
	float GroundStrongAttackStunChance = 0.8f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat|Stun",
		meta = (ClampMin = "0.0", ClampMax = "1.0")
	)
	float AirStrongAttackStunChance = 0.4f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Combat",
		meta = (ClampMin = "0.01", Units = "s")
	)
	float AttackCooldown = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Debug")
	bool bDrawAttackDebug = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	TObjectPtr<UStaminaComponent> StaminaComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;
	bool bDeathPresentationStarted = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health")
	bool bIsRespawning = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health")
	bool bIsGameOver = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Game Completion")
	bool bIsGameCompleted = false;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Fall Damage",
		meta = (ClampMin = "0.0", Units = "cm/s")
	)
	float LightFallImpactSpeed = 1365.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Fall Damage",
		meta = (ClampMin = "0.0", Units = "cm/s")
	)
	float MediumFallImpactSpeed = 1565.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Fall Damage",
		meta = (ClampMin = "0.0", Units = "cm/s")
	)
	float HeavyFallImpactSpeed = 1800.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Fall Damage",
		meta = (ClampMin = "0.0", Units = "cm/s")
	)
	float FatalFallImpactSpeed = 2055.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Fall Damage",
		meta = (ClampMin = "0.0")
	)
	float LightFallDamage = 25.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Fall Damage",
		meta = (ClampMin = "0.0")
	)
	float MediumFallDamage = 50.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Fall Damage",
		meta = (ClampMin = "0.0")
	)
	float HeavyFallDamage = 75.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Lives",
		meta = (ClampMin = "1")
	)
	int32 MaxLives = 3;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health|Lives")
	int32 RemainingLives = 3;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat|Stun")
	bool bIsStunned = false;

	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> RunningAudioComponent;

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
	float HitReactDuration = 0.15f;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health|Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health|Animation")
	TObjectPtr<UAnimMontage> RespawnMontage;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health",
		meta = (ClampMin = "0.0", Units = "s")
	)
	float DeathRestartDelay = 2.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Game Over",
		meta = (ClampMin = "0.0", Units = "s")
	)
	float GameOverScreenDelay = 0.75f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Health|Respawn",
		meta = (ClampMin = "0.0", Units = "s")
	)
	float RespawnFallbackDuration = 1.0f;

	FTransform DeathTransform = FTransform::Identity;

	FTimerHandle RespawnTimerHandle;
	FTimerHandle StunTimerHandle;
	FTimerHandle ParryWindowTimerHandle;
	FTimerHandle HitReactTimerHandle;
	FTimerHandle HitFlashTimerHandle;
};
