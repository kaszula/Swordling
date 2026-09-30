#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DiplomaGameGameInstance.generated.h"

class USupabaseService;

UCLASS()
class DIPLOMAGAME_API UDiplomaGameGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void RequestPlayerRespawn();
	bool ConsumePlayerRespawnRequest();
	void StartGameSession();
	void RecordFangKill();
	void RecordPlayerDamage(float DamageAmount);
	void CompleteGameSession(
		const FString& PlayerName,
		const FString& HeroName
	);
	bool GetLastCompletedSessionResult(
		FString& OutPlayerName,
		FString& OutHeroName,
		int64& OutCompletionTimeMs,
		int32& OutFangsKilled,
		int32& OutDamageTaken
	) const;
	USupabaseService* GetSupabaseService() const;

	UFUNCTION(BlueprintCallable, Exec, Category = "Supabase|Test")
	void TestGetSupabaseLeaderboard();

private:
	UPROPERTY(Transient)
	bool bPlayerRespawnRequested = false;

	UPROPERTY(Transient)
	TObjectPtr<USupabaseService> SupabaseService;

	double SessionStartTimeSeconds = 0.0;
	float SessionDamageTaken = 0.0f;
	int32 SessionFangsKilled = 0;
	bool bSessionInProgress = false;
	bool bSessionSubmitted = false;

	FString LastCompletedPlayerName;
	FString LastCompletedHeroName;
	int64 LastCompletionTimeMs = 0;
	int32 LastSessionFangsKilled = 0;
	int32 LastSessionDamageTaken = 0;
	bool bHasCompletedSessionResult = false;
};
