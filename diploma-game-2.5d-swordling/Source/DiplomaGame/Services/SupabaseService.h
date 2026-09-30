#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "UObject/Object.h"
#include "SupabaseService.generated.h"

USTRUCT(BlueprintType)
struct FLeaderboardEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Leaderboard")
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly, Category = "Leaderboard")
	FString Hero;

	UPROPERTY(BlueprintReadOnly, Category = "Leaderboard")
	int64 CompletionTimeMs = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Leaderboard")
	int32 EnemiesKilled = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Leaderboard")
	int32 DamageTaken = 0;
};

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnLeaderboardUpdated,
	const TArray<FLeaderboardEntry>&
);

UCLASS()
class DIPLOMAGAME_API USupabaseService : public UObject
{
	GENERATED_BODY()

public:
	void SubmitGameSession(
		const FString& PlayerName,
		const FString& HeroName,
		int64 CompletionTimeMs,
		int32 EnemiesKilled,
		int32 DamageTaken
	);
	void GetLeaderboard();

	const TArray<FLeaderboardEntry>& GetCachedLeaderboard() const;
	bool IsSubmissionInProgress() const;
	bool IsLeaderboardRequestInProgress() const;

	FOnLeaderboardUpdated OnLeaderboardUpdated;

private:
	using FResponseHandler = TFunction<void(
		bool,
		FHttpResponsePtr,
		const FString&
	)>;

	struct FPendingSessionSubmission
	{
		FString PlayerName;
		FString HeroName;
		int64 CompletionTimeMs = 0;
		int32 EnemiesKilled = 0;
		int32 DamageTaken = 0;
		int64 PlayerId = 0;
		int64 HeroId = 0;
		int64 EnemyTypeId = 0;
		int64 SessionId = 0;
	};

	void ResolvePlayer();
	void CreatePlayer();
	void ResolveHero();
	void ResolveEnemyType();
	void CreateGameSession();
	void CreateSessionKills();
	void FinishSubmission();
	void FailSubmission(const FString& Reason, const FString& ResponseBody);

	void SendRequest(
		const FString& OperationName,
		const FString& Endpoint,
		const FString& Verb,
		const FString& RequestBody,
		bool bReturnRepresentation,
		FResponseHandler ResponseHandler
	);
	bool TryParseFirstId(
		const FString& ResponseBody,
		int64& OutId,
		bool& bOutHasRecord
	) const;
	FString SerializeJsonObject(
		const TSharedRef<class FJsonObject>& JsonObject
	) const;
	void HandleLeaderboardResponse(
		bool bWasSuccessful,
		FHttpResponsePtr Response,
		const FString& ResponseBody
	);

	FPendingSessionSubmission PendingSubmission;
	TArray<FLeaderboardEntry> CachedLeaderboard;
	bool bSubmissionInProgress = false;
	bool bLeaderboardRequestInProgress = false;
};
