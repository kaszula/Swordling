#include "DiplomaGameGameInstance.h"

#include "DiplomaGame.h"
#include "HAL/PlatformTime.h"
#include "Services/SupabaseService.h"

void UDiplomaGameGameInstance::Init()
{
	Super::Init();

	SupabaseService = NewObject<USupabaseService>(this);
}

void UDiplomaGameGameInstance::RequestPlayerRespawn()
{
	bPlayerRespawnRequested = true;
}

bool UDiplomaGameGameInstance::ConsumePlayerRespawnRequest()
{
	const bool bWasRequested = bPlayerRespawnRequested;
	bPlayerRespawnRequested = false;
	return bWasRequested;
}

void UDiplomaGameGameInstance::StartGameSession()
{
	SessionStartTimeSeconds = FPlatformTime::Seconds();
	SessionDamageTaken = 0.0f;
	SessionFangsKilled = 0;
	bSessionInProgress = true;
	bSessionSubmitted = false;
	bHasCompletedSessionResult = false;

	UE_LOG(LogDiplomaGame, Display, TEXT("Game session tracking started"));
}

void UDiplomaGameGameInstance::RecordFangKill()
{
	if (bSessionInProgress)
	{
		++SessionFangsKilled;
	}
}

void UDiplomaGameGameInstance::RecordPlayerDamage(const float DamageAmount)
{
	if (bSessionInProgress && DamageAmount > 0.0f)
	{
		SessionDamageTaken += DamageAmount;
	}
}

void UDiplomaGameGameInstance::CompleteGameSession(
	const FString& PlayerName,
	const FString& HeroName
)
{
	if (!bSessionInProgress || bSessionSubmitted)
	{
		return;
	}

	bSessionInProgress = false;
	bSessionSubmitted = true;

	const double ElapsedSeconds = FMath::Max(
		FPlatformTime::Seconds() - SessionStartTimeSeconds,
		0.0
	);
	const int64 CompletionTimeMs = FMath::RoundToInt64(
		ElapsedSeconds * 1000.0
	);
	const int32 DamageTaken = FMath::Max(
		FMath::RoundToInt(SessionDamageTaken),
		0
	);

	LastCompletedPlayerName = PlayerName;
	LastCompletedHeroName = HeroName;
	LastCompletionTimeMs = CompletionTimeMs;
	LastSessionFangsKilled = SessionFangsKilled;
	LastSessionDamageTaken = DamageTaken;
	bHasCompletedSessionResult = true;

	if (!IsValid(SupabaseService))
	{
		SupabaseService = NewObject<USupabaseService>(this);
	}

	UE_LOG(
		LogDiplomaGame,
		Display,
		TEXT(
			"Submitting game session: player=%s, hero=%s, "
			"time_ms=%lld, fangs=%d, damage=%d"
		),
		*PlayerName,
		*HeroName,
		CompletionTimeMs,
		SessionFangsKilled,
		DamageTaken
	);

	SupabaseService->SubmitGameSession(
		PlayerName,
		HeroName,
		CompletionTimeMs,
		SessionFangsKilled,
		DamageTaken
	);
}

bool UDiplomaGameGameInstance::GetLastCompletedSessionResult(
	FString& OutPlayerName,
	FString& OutHeroName,
	int64& OutCompletionTimeMs,
	int32& OutFangsKilled,
	int32& OutDamageTaken
) const
{
	if (!bHasCompletedSessionResult)
	{
		return false;
	}

	OutPlayerName = LastCompletedPlayerName;
	OutHeroName = LastCompletedHeroName;
	OutCompletionTimeMs = LastCompletionTimeMs;
	OutFangsKilled = LastSessionFangsKilled;
	OutDamageTaken = LastSessionDamageTaken;
	return true;
}

USupabaseService* UDiplomaGameGameInstance::GetSupabaseService() const
{
	return SupabaseService;
}

void UDiplomaGameGameInstance::TestGetSupabaseLeaderboard()
{
	if (!IsValid(SupabaseService))
	{
		SupabaseService = NewObject<USupabaseService>(this);
	}

	SupabaseService->GetLeaderboard();
}
