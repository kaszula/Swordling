#include "Characters/Player/PlayerCharacterBase.h"

#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Components/HealthComponent.h"
#include "DiplomaGameGameInstance.h"
#include "Components/StaminaComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DiplomaGame.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TitleRevealWidget.h"
#include "UObject/UnrealType.h"

APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bUseControllerRotationYaw = false;
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(
		TEXT("StaminaComponent")
	);
	MenuMusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(
		TEXT("MenuMusicAudioComponent")
	);
	MenuMusicAudioComponent->SetupAttachment(RootComponent);
	MenuMusicAudioComponent->bAutoActivate = false;
	MenuMusicAudioComponent->bAllowSpatialization = false;
	MenuMusicAudioComponent->bIsUISound = true;

	TinyHeroBoyStats.MaxLives = 2;
	TinyHeroBoyStats.HealthMultiplier = 1.0f;
	TinyHeroBoyStats.StaminaMultiplier = 1.0f;
	TinyHeroBoyStats.AttackMultiplier = 0.8f;

	TinyHeroGirlStats.MaxLives = 3;
	TinyHeroGirlStats.HealthMultiplier = 0.6f;
	TinyHeroGirlStats.StaminaMultiplier = 0.8f;
	TinyHeroGirlStats.AttackMultiplier = 1.0f;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
	MovementComponent->bSnapToPlaneAtStart = true;
	MovementComponent->bOrientRotationToMovement = false;
	MovementComponent->AirControl = 0.6f;
	JumpMaxCount = 2;
}

void APlayerCharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ApplySelectedAvatar();
}

void APlayerCharacterBase::SetPlayerAvatar(
	const EPlayerAvatar NewAvatar
)
{
	SelectedAvatar = NewAvatar;
	ApplySelectedAvatar();
	ApplySelectedAvatarStats();
}

EPlayerAvatar APlayerCharacterBase::GetPlayerAvatar() const
{
	return SelectedAvatar;
}

void APlayerCharacterBase::RecordFangKill()
{
	if (UDiplomaGameGameInstance* GameInstance =
		GetGameInstance<UDiplomaGameGameInstance>())
	{
		GameInstance->RecordFangKill();
	}
}

FString APlayerCharacterBase::GetEnteredPlayerName() const
{
	const FProperty* PlayerNameProperty = GetClass()->FindPropertyByName(
		TEXT("PlayerName")
	);
	if (const FTextProperty* TextProperty =
		CastField<FTextProperty>(PlayerNameProperty))
	{
		return TextProperty->GetPropertyValue_InContainer(this)
			.ToString()
			.TrimStartAndEnd();
	}

	if (const FStrProperty* StringProperty =
		CastField<FStrProperty>(PlayerNameProperty))
	{
		return StringProperty->GetPropertyValue_InContainer(this)
			.TrimStartAndEnd();
	}

	return FString();
}

FString APlayerCharacterBase::GetSelectedHeroName() const
{
	return SelectedAvatar == EPlayerAvatar::TinyHeroGirl
		? TinyHeroGirlDatabaseName
		: TinyHeroBoyDatabaseName;
}

void APlayerCharacterBase::ApplySelectedAvatar()
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (!IsValid(CharacterMesh))
	{
		return;
	}

	USkeletalMesh* SelectedMesh = nullptr;

	switch (SelectedAvatar)
	{
	case EPlayerAvatar::TinyHeroBoy:
		SelectedMesh = TinyHeroBoyMesh.Get();
		break;

	case EPlayerAvatar::TinyHeroGirl:
		SelectedMesh = TinyHeroGirlMesh.Get();
		break;

	default:
		break;
	}

	if (!IsValid(SelectedMesh))
	{
		return;
	}

	CharacterMesh->SetSkeletalMesh(SelectedMesh, true);

	if (PlayerAnimationClass)
	{
		CharacterMesh->SetAnimInstanceClass(PlayerAnimationClass);
	}
}

void APlayerCharacterBase::CacheBaseAvatarStats()
{
	if (bBaseAvatarStatsCached)
	{
		return;
	}

	if (!IsValid(HealthComponent) || !IsValid(StaminaComponent))
	{
		return;
	}

	BaseMaxHealth = FMath::Max(HealthComponent->GetMaxHealth(), 0.0f);
	BaseMaxStamina = FMath::Max(StaminaComponent->GetMaxStamina(), 0.0f);
	BaseAttackDamage = FMath::Max(AttackDamage, 0.0f);
	bBaseAvatarStatsCached = true;
}

const FPlayerAvatarStats& APlayerCharacterBase::GetSelectedAvatarStats() const
{
	switch (SelectedAvatar)
	{
	case EPlayerAvatar::TinyHeroGirl:
		return TinyHeroGirlStats;

	case EPlayerAvatar::TinyHeroBoy:
	default:
		return TinyHeroBoyStats;
	}
}

void APlayerCharacterBase::ApplySelectedAvatarStats()
{
	CacheBaseAvatarStats();
	if (!bBaseAvatarStatsCached)
	{
		return;
	}

	const FPlayerAvatarStats& Stats = GetSelectedAvatarStats();
	const float HealthMultiplier = FMath::Max(
		Stats.HealthMultiplier,
		0.0f
	);
	const float StaminaMultiplier = FMath::Max(
		Stats.StaminaMultiplier,
		0.0f
	);
	const float AttackMultiplier = FMath::Max(
		Stats.AttackMultiplier,
		0.0f
	);

	MaxLives = FMath::Max(Stats.MaxLives, 1);
	RemainingLives = MaxLives;
	AttackDamage = BaseAttackDamage * AttackMultiplier;

	const float ProfileMaxHealth = BaseMaxHealth * HealthMultiplier;
	HealthComponent->SetHealthValues(
		ProfileMaxHealth,
		ProfileMaxHealth
	);

	const float ProfileMaxStamina = BaseMaxStamina * StaminaMultiplier;
	StaminaComponent->SetStaminaValues(
		ProfileMaxStamina,
		ProfileMaxStamina
	);

	OnLivesChanged.Broadcast(RemainingLives);
}

void APlayerCharacterBase::BeginPlay()
{
	HealthComponent = FindComponentByClass<UHealthComponent>();
	Super::BeginPlay();
	CacheBaseAvatarStats();
	ApplySelectedAvatar();
	ApplySelectedAvatarStats();

	if (HealthComponent)
	{
		HealthComponent->OnDamageApplied.AddUObject(
			this,
			&APlayerCharacterBase::HandleDamageApplied
		);
		HealthComponent->OnDamaged.AddUObject(
			this,
			&APlayerCharacterBase::HandleDamaged
		);
		HealthComponent->OnDeath.AddUObject(
			this,
			&APlayerCharacterBase::HandleDeath
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
	else
	{
		UE_LOG(
			LogDiplomaGameCombat,
			Error,
			TEXT("%s requires a HealthComponent"),
			*GetName()
		);
	}

	InitialMeshRelativeLocation = GetMesh()->GetRelativeLocation();
	InitialMeshWorldYaw = GetMesh()->GetComponentRotation().Yaw;
	MeshFacingYawOffset = FMath::FindDeltaAngleDegrees(
		GetActorRotation().Yaw,
		InitialMeshWorldYaw
	);
	TargetMeshYaw = InitialMeshWorldYaw;

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()
		);

	if (InputSubsystem && DefaultMappingContext)
	{
		InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	ShowTitleScreen();
}

void APlayerCharacterBase::ShowTitleScreen()
{
	if (!IsLocallyControlled() || !TitleScreenWidgetClass || TitleScreenWidget)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}

	TitleScreenWidget = CreateWidget<UUserWidget>(
		PlayerController,
		TitleScreenWidgetClass
	);
	if (!TitleScreenWidget)
	{
		return;
	}

	TitleScreenWidget->AddToViewport(100);
	PlayMenuMusic(false);
	PlayerController->SetIgnoreMoveInput(true);
	PlayerController->SetIgnoreLookInput(true);
	PlayerController->bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
	UGameplayStatics::SetGamePaused(this, true);
}

bool APlayerCharacterBase::StartTitleReveal()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return false;
	}

	UGameplayStatics::SetGamePaused(this, false);
	StopMenuMusic();
	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(FInputModeGameOnly());

	if (!TitleRevealWidgetClass)
	{
		FinishTitleReveal();
		return false;
	}

	TitleRevealWidget = CreateWidget<UTitleRevealWidget>(
		PlayerController,
		TitleRevealWidgetClass
	);
	if (!TitleRevealWidget)
	{
		FinishTitleReveal();
		return false;
	}

	TitleRevealWidget->AddToViewport(200);
	TitleRevealWidget->StartReveal(this);
	return true;
}

void APlayerCharacterBase::ApplyTitleRevealMovement(
	const float MovementScale
)
{
	if (bIsDead || bIsRespawning || bIsGameOver)
	{
		return;
	}

	UpdateMeshFacing(MovementScale);
	UpdateRunningSound(MovementScale);
	AddMovementInput(FVector::ForwardVector, MovementScale, true);
}

void APlayerCharacterBase::StopTitleRevealMovement()
{
	StopRunningSound();
}

void APlayerCharacterBase::FinishTitleReveal()
{
	StopRunningSound();
	TitleRevealWidget = nullptr;

	if (APlayerController* PlayerController =
		Cast<APlayerController>(Controller))
	{
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	if (UDiplomaGameGameInstance* GameInstance =
		GetGameInstance<UDiplomaGameGameInstance>())
	{
		GameInstance->StartGameSession();
	}
}

bool APlayerCharacterBase::StartGameCompletion(
	TSubclassOf<UTitleRevealWidget> EndRevealWidgetClass,
	TSubclassOf<UUserWidget> ScoreboardWidgetClass
)
{
	if (bIsGameCompleted
		|| bIsDead
		|| bIsRespawning
		|| bIsGameOver
		|| !EndRevealWidgetClass
		|| !ScoreboardWidgetClass)
	{
		return false;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return false;
	}

	UTitleRevealWidget* EndRevealWidget =
		CreateWidget<UTitleRevealWidget>(
			PlayerController,
			EndRevealWidgetClass
		);
	if (!EndRevealWidget)
	{
		return false;
	}

	bIsGameCompleted = true;
	CompletionScoreboardWidgetClass = ScoreboardWidgetClass;
	TitleRevealWidget = EndRevealWidget;

	if (UDiplomaGameGameInstance* GameInstance =
		GetGameInstance<UDiplomaGameGameInstance>())
	{
		GameInstance->CompleteGameSession(
			GetEnteredPlayerName(),
			GetSelectedHeroName()
		);
	}

	if (HealthComponent)
	{
		HealthComponent->SetInvulnerable(true);
	}

	PlayerController->SetIgnoreMoveInput(true);
	PlayerController->SetIgnoreLookInput(true);
	PlayerController->bShowMouseCursor = false;
	DisableInput(PlayerController);

	TitleRevealWidget->AddToViewport(200);
	TitleRevealWidget->StartClosingReveal(this);
	return true;
}

bool APlayerCharacterBase::IsGameCompleted() const
{
	return bIsGameCompleted;
}

void APlayerCharacterBase::FinishGameCompletion()
{
	StopRunningSound();
	TitleRevealWidget = nullptr;
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController && CompletionScoreboardWidgetClass)
	{
		ScoreboardWidget = CreateWidget<UUserWidget>(
			PlayerController,
			CompletionScoreboardWidgetClass
		);
		if (ScoreboardWidget)
		{
			ScoreboardWidget->AddToViewport(100);
		}

		PlayerController->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
	}

	PlayMenuMusic(true);
	UGameplayStatics::SetGamePaused(this, true);

	OnGameCompleted.Broadcast();

	UE_LOG(
		LogDiplomaGame,
		Display,
		TEXT("Game completed. Scoreboard displayed")
	);
}

void APlayerCharacterBase::PlayMenuMusic(
	const bool bRestartFromBeginning
)
{
	if (!MenuMusicAudioComponent || !MenuMusicAudioComponent->Sound)
	{
		return;
	}

	if (bRestartFromBeginning)
	{
		MenuMusicAudioComponent->Stop();
		MenuMusicAudioComponent->Play(0.0f);
		return;
	}

	if (!MenuMusicAudioComponent->IsPlaying())
	{
		MenuMusicAudioComponent->Play(0.0f);
	}
}

void APlayerCharacterBase::StopMenuMusic()
{
	if (MenuMusicAudioComponent && MenuMusicAudioComponent->IsPlaying())
	{
		MenuMusicAudioComponent->Stop();
	}
}

UHealthComponent* APlayerCharacterBase::GetHealthComponent() const
{
	return HealthComponent;
}

UStaminaComponent* APlayerCharacterBase::GetStaminaComponent() const
{
	return StaminaComponent;
}

bool APlayerCharacterBase::IsDead() const
{
	return bIsDead;
}

int32 APlayerCharacterBase::GetRemainingLives() const
{
	return RemainingLives;
}

int32 APlayerCharacterBase::GetMaxLives() const
{
	return MaxLives;
}

bool APlayerCharacterBase::IsRespawning() const
{
	return bIsRespawning;
}

bool APlayerCharacterBase::IsGameOver() const
{
	return bIsGameOver;
}
