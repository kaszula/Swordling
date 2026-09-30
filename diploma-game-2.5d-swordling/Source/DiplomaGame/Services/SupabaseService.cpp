#include "Services/SupabaseService.h"

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "HttpModule.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

DEFINE_LOG_CATEGORY_STATIC(LogSupabase, Log, All);

namespace SupabaseConstants
{
	const FString ProjectUrl =
		TEXT("https://jlqvvycbrcqeeokulohs.supabase.co");
	const FString PublishableApiKey =
		TEXT("sb_publishable_j0Du8cJE9ELjZrDVwnv1vA_mj3fZXzP");
	const FString LeaderboardEndpoint = TEXT(
		"/rest/v1/leaderboard?"
		"select=player_name,hero,completion_time_ms,"
		"enemies_killed,damage_taken&order=score.desc&limit=10"
	);
}

void USupabaseService::SubmitGameSession(
	const FString& PlayerName,
	const FString& HeroName,
	const int64 CompletionTimeMs,
	const int32 EnemiesKilled,
	const int32 DamageTaken
)
{
	if (bSubmissionInProgress)
	{
		UE_LOG(
			LogSupabase,
			Warning,
			TEXT("Submit game session ignored: submission already in progress")
		);
		return;
	}

	const FString TrimmedPlayerName = PlayerName.TrimStartAndEnd();
	if (TrimmedPlayerName.IsEmpty()
		|| (HeroName != TEXT("Willow") && HeroName != TEXT("Tristan"))
		|| CompletionTimeMs < 0
		|| EnemiesKilled < 0
		|| DamageTaken < 0)
	{
		UE_LOG(
			LogSupabase,
			Error,
			TEXT("Submit game session failed: invalid session data")
		);
		return;
	}

	PendingSubmission = FPendingSessionSubmission();
	PendingSubmission.PlayerName = TrimmedPlayerName;
	PendingSubmission.HeroName = HeroName;
	PendingSubmission.CompletionTimeMs = CompletionTimeMs;
	PendingSubmission.EnemiesKilled = EnemiesKilled;
	PendingSubmission.DamageTaken = DamageTaken;
	bSubmissionInProgress = true;

	ResolvePlayer();
}

void USupabaseService::ResolvePlayer()
{
	const FString Endpoint = FString::Printf(
		TEXT("/rest/v1/players?select=id&name=eq.%s&limit=1"),
		*FGenericPlatformHttp::UrlEncode(PendingSubmission.PlayerName)
	);

	SendRequest(
		TEXT("Find player"), Endpoint, TEXT("GET"), FString(), false,
		[this](const bool bSuccess, FHttpResponsePtr, const FString& Body)
		{
			if (!bSuccess)
			{
				FailSubmission(TEXT("could not find player"), Body);
				return;
			}

			bool bHasRecord = false;
			if (!TryParseFirstId(Body, PendingSubmission.PlayerId, bHasRecord))
			{
				FailSubmission(TEXT("invalid player response"), Body);
				return;
			}

			if (bHasRecord)
			{
				ResolveHero();
			}
			else
			{
				CreatePlayer();
			}
		}
	);
}

void USupabaseService::CreatePlayer()
{
	const TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField(TEXT("name"), PendingSubmission.PlayerName);

	SendRequest(
		TEXT("Create player"),
		TEXT("/rest/v1/players?select=id"),
		TEXT("POST"),
		SerializeJsonObject(JsonObject),
		true,
		[this](const bool bSuccess, FHttpResponsePtr, const FString& Body)
		{
			bool bHasRecord = false;
			if (!bSuccess
				|| !TryParseFirstId(Body, PendingSubmission.PlayerId, bHasRecord)
				|| !bHasRecord)
			{
				FailSubmission(TEXT("could not create player"), Body);
				return;
			}

			ResolveHero();
		}
	);
}

void USupabaseService::ResolveHero()
{
	const FString Endpoint = FString::Printf(
		TEXT("/rest/v1/heroes?select=id&name=eq.%s&limit=1"),
		*FGenericPlatformHttp::UrlEncode(PendingSubmission.HeroName)
	);

	SendRequest(
		TEXT("Find hero"), Endpoint, TEXT("GET"), FString(), false,
		[this](const bool bSuccess, FHttpResponsePtr, const FString& Body)
		{
			bool bHasRecord = false;
			if (!bSuccess
				|| !TryParseFirstId(Body, PendingSubmission.HeroId, bHasRecord)
				|| !bHasRecord)
			{
				FailSubmission(TEXT("could not resolve hero id"), Body);
				return;
			}

			ResolveEnemyType();
		}
	);
}

void USupabaseService::ResolveEnemyType()
{
	SendRequest(
		TEXT("Find Fang enemy type"),
		TEXT("/rest/v1/enemy_types?select=id&name=eq.Fang&limit=1"),
		TEXT("GET"),
		FString(),
		false,
		[this](const bool bSuccess, FHttpResponsePtr, const FString& Body)
		{
			bool bHasRecord = false;
			if (!bSuccess
				|| !TryParseFirstId(
					Body,
					PendingSubmission.EnemyTypeId,
					bHasRecord
				)
				|| !bHasRecord)
			{
				FailSubmission(
					TEXT("could not resolve Fang enemy type id"),
					Body
				);
				return;
			}

			CreateGameSession();
		}
	);
}

void USupabaseService::CreateGameSession()
{
	const TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetNumberField(
		TEXT("player_id"),
		static_cast<double>(PendingSubmission.PlayerId)
	);
	JsonObject->SetNumberField(
		TEXT("hero_id"),
		static_cast<double>(PendingSubmission.HeroId)
	);
	JsonObject->SetNumberField(
		TEXT("completion_time_ms"),
		static_cast<double>(PendingSubmission.CompletionTimeMs)
	);
	JsonObject->SetNumberField(
		TEXT("damage_taken"),
		PendingSubmission.DamageTaken
	);

	SendRequest(
		TEXT("Create game session"),
		TEXT("/rest/v1/game_sessions?select=id"),
		TEXT("POST"),
		SerializeJsonObject(JsonObject),
		true,
		[this](const bool bSuccess, FHttpResponsePtr, const FString& Body)
		{
			bool bHasRecord = false;
			if (!bSuccess
				|| !TryParseFirstId(Body, PendingSubmission.SessionId, bHasRecord)
				|| !bHasRecord)
			{
				FailSubmission(TEXT("could not create game session"), Body);
				return;
			}

			CreateSessionKills();
		}
	);
}

void USupabaseService::CreateSessionKills()
{
	const TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetNumberField(
		TEXT("session_id"),
		static_cast<double>(PendingSubmission.SessionId)
	);
	JsonObject->SetNumberField(
		TEXT("enemy_type_id"),
		static_cast<double>(PendingSubmission.EnemyTypeId)
	);
	JsonObject->SetNumberField(
		TEXT("kill_count"),
		PendingSubmission.EnemiesKilled
	);

	SendRequest(
		TEXT("Create session kills"),
		TEXT("/rest/v1/session_kills"),
		TEXT("POST"),
		SerializeJsonObject(JsonObject),
		false,
		[this](const bool bSuccess, FHttpResponsePtr, const FString& Body)
		{
			if (!bSuccess)
			{
				FailSubmission(TEXT("could not create session kills"), Body);
				return;
			}

			FinishSubmission();
		}
	);
}

void USupabaseService::FinishSubmission()
{
	bSubmissionInProgress = false;
	UE_LOG(LogSupabase, Display, TEXT("Submit game session succeeded"));
	GetLeaderboard();
}

void USupabaseService::FailSubmission(
	const FString& Reason,
	const FString& ResponseBody
)
{
	bSubmissionInProgress = false;
	UE_LOG(LogSupabase, Error, TEXT("Submit game session failed: %s"), *Reason);
	if (!ResponseBody.IsEmpty())
	{
		UE_LOG(LogSupabase, Error, TEXT("Error response body:\n%s"), *ResponseBody);
	}

	GetLeaderboard();
}

void USupabaseService::GetLeaderboard()
{
	if (bLeaderboardRequestInProgress)
	{
		return;
	}

	bLeaderboardRequestInProgress = true;
	SendRequest(
		TEXT("Get leaderboard TOP 10"),
		SupabaseConstants::LeaderboardEndpoint,
		TEXT("GET"),
		FString(),
		false,
		[this](
			const bool bSuccess,
			FHttpResponsePtr Response,
			const FString& Body
		)
		{
			HandleLeaderboardResponse(bSuccess, Response, Body);
		}
	);
}

const TArray<FLeaderboardEntry>&
USupabaseService::GetCachedLeaderboard() const
{
	return CachedLeaderboard;
}

bool USupabaseService::IsSubmissionInProgress() const
{
	return bSubmissionInProgress;
}

bool USupabaseService::IsLeaderboardRequestInProgress() const
{
	return bLeaderboardRequestInProgress;
}

void USupabaseService::SendRequest(
	const FString& OperationName,
	const FString& Endpoint,
	const FString& Verb,
	const FString& RequestBody,
	const bool bReturnRepresentation,
	FResponseHandler ResponseHandler
)
{
	const FString RequestUrl = SupabaseConstants::ProjectUrl + Endpoint;
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().CreateRequest();

	Request->SetURL(RequestUrl);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("apikey"), SupabaseConstants::PublishableApiKey);
	Request->SetHeader(
		TEXT("Authorization"),
		TEXT("Bearer ") + SupabaseConstants::PublishableApiKey
	);
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	if (!RequestBody.IsEmpty())
	{
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetContentAsString(RequestBody);
	}
	if (bReturnRepresentation)
	{
		Request->SetHeader(TEXT("Prefer"), TEXT("return=representation"));
	}

	Request->OnProcessRequestComplete().BindWeakLambda(
		this,
		[OperationName, Endpoint, ResponseHandler](
			FHttpRequestPtr,
			FHttpResponsePtr Response,
			const bool bTransportSuccessful
		)
		{
			const int32 StatusCode = Response.IsValid()
				? Response->GetResponseCode()
				: 0;
			const FString Body = Response.IsValid()
				? Response->GetContentAsString()
				: FString();
			const bool bSuccess = bTransportSuccessful
				&& Response.IsValid()
				&& EHttpResponseCodes::IsOk(StatusCode);

			UE_LOG(LogSupabase, Log, TEXT("Operation: %s"), *OperationName);
			UE_LOG(LogSupabase, Log, TEXT("Endpoint: %s"), *Endpoint);
			UE_LOG(LogSupabase, Log, TEXT("HTTP status code: %d"), StatusCode);
			if (bSuccess)
			{
				UE_LOG(LogSupabase, Display, TEXT("Result: success"));
			}
			else
			{
				UE_LOG(LogSupabase, Error, TEXT("Result: error"));
				UE_LOG(
					LogSupabase,
					Error,
					TEXT("Error response body:\n%s"),
					Body.IsEmpty() ? TEXT("<no response body>") : *Body
				);
			}

			ResponseHandler(bSuccess, Response, Body);
		}
	);

	UE_LOG(LogSupabase, Log, TEXT("Starting operation: %s"), *OperationName);
	UE_LOG(LogSupabase, Log, TEXT("Endpoint: %s"), *Endpoint);

	if (!Request->ProcessRequest())
	{
		Request->OnProcessRequestComplete().Unbind();
		UE_LOG(LogSupabase, Error, TEXT("Operation: %s"), *OperationName);
		UE_LOG(LogSupabase, Error, TEXT("Endpoint: %s"), *Endpoint);
		UE_LOG(LogSupabase, Error, TEXT("HTTP status code: 0"));
		UE_LOG(LogSupabase, Error, TEXT("Result: error"));
		UE_LOG(
			LogSupabase,
			Error,
			TEXT("Error response body: <request not started>")
		);
		ResponseHandler(false, nullptr, FString());
	}
}

bool USupabaseService::TryParseFirstId(
	const FString& ResponseBody,
	int64& OutId,
	bool& bOutHasRecord
) const
{
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	const TSharedRef<TJsonReader<>> Reader =
		TJsonReaderFactory<>::Create(ResponseBody);
	if (!FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		return false;
	}

	bOutHasRecord = JsonArray.Num() > 0;
	if (!bOutHasRecord)
	{
		OutId = 0;
		return true;
	}

	const TSharedPtr<FJsonObject> JsonObject = JsonArray[0]->AsObject();
	if (!JsonObject.IsValid())
	{
		return false;
	}

	double IdValue = 0.0;
	if (!JsonObject->TryGetNumberField(TEXT("id"), IdValue))
	{
		return false;
	}

	OutId = static_cast<int64>(IdValue);
	return OutId > 0;
}

FString USupabaseService::SerializeJsonObject(
	const TSharedRef<FJsonObject>& JsonObject
) const
{
	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer =
		TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject, Writer);
	return JsonString;
}

void USupabaseService::HandleLeaderboardResponse(
	const bool bWasSuccessful,
	FHttpResponsePtr,
	const FString& ResponseBody
)
{
	bLeaderboardRequestInProgress = false;
	if (!bWasSuccessful)
	{
		return;
	}

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	const TSharedRef<TJsonReader<>> Reader =
		TJsonReaderFactory<>::Create(ResponseBody);
	if (!FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		UE_LOG(LogSupabase, Error, TEXT("Leaderboard JSON parsing failed"));
		UE_LOG(LogSupabase, Error, TEXT("Error response body:\n%s"), *ResponseBody);
		return;
	}

	TArray<FLeaderboardEntry> ParsedEntries;
	ParsedEntries.Reserve(FMath::Min(JsonArray.Num(), 10));
	for (int32 Index = 0; Index < JsonArray.Num() && Index < 10; ++Index)
	{
		const TSharedPtr<FJsonObject> JsonObject = JsonArray[Index]->AsObject();
		if (!JsonObject.IsValid())
		{
			UE_LOG(LogSupabase, Error, TEXT("Leaderboard entry %d is invalid"), Index);
			UE_LOG(LogSupabase, Error, TEXT("Error response body:\n%s"), *ResponseBody);
			return;
		}

		FLeaderboardEntry Entry;
		double CompletionTimeValue = 0.0;
		double EnemiesKilledValue = 0.0;
		double DamageTakenValue = 0.0;
		if (!JsonObject->TryGetStringField(TEXT("player_name"), Entry.PlayerName)
			|| !JsonObject->TryGetStringField(TEXT("hero"), Entry.Hero)
			|| !JsonObject->TryGetNumberField(
				TEXT("completion_time_ms"), CompletionTimeValue
			)
			|| !JsonObject->TryGetNumberField(
				TEXT("enemies_killed"), EnemiesKilledValue
			)
			|| !JsonObject->TryGetNumberField(
				TEXT("damage_taken"), DamageTakenValue
			))
		{
			UE_LOG(
				LogSupabase,
				Error,
				TEXT("Leaderboard entry %d is missing required fields"),
				Index
			);
			UE_LOG(LogSupabase, Error, TEXT("Error response body:\n%s"), *ResponseBody);
			return;
		}

		Entry.CompletionTimeMs = static_cast<int64>(CompletionTimeValue);
		Entry.EnemiesKilled = FMath::RoundToInt(EnemiesKilledValue);
		Entry.DamageTaken = FMath::RoundToInt(DamageTakenValue);
		ParsedEntries.Add(MoveTemp(Entry));
	}

	CachedLeaderboard = MoveTemp(ParsedEntries);
	UE_LOG(
		LogSupabase,
		Display,
		TEXT("Leaderboard TOP 10 parsed: %d entries"),
		CachedLeaderboard.Num()
	);
	OnLeaderboardUpdated.Broadcast(CachedLeaderboard);
}
