#include "UI/ScoreboardScreenWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "DiplomaGameGameInstance.h"
#include "Engine/World.h"
#include "Services/SupabaseService.h"
#include "UI/ScoreboardRowWidget.h"

void UScoreboardScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	UDiplomaGameGameInstance* GameInstance = World
		? World->GetGameInstance<UDiplomaGameGameInstance>()
		: nullptr;
	if (!GameInstance)
	{
		return;
	}

	PopulateCurrentSession(GameInstance);

	SupabaseService = GameInstance->GetSupabaseService();
	if (!SupabaseService)
	{
		return;
	}

	SupabaseService->OnLeaderboardUpdated.RemoveAll(this);
	SupabaseService->OnLeaderboardUpdated.AddUObject(
		this,
		&UScoreboardScreenWidget::PopulateLeaderboard
	);

	const TArray<FLeaderboardEntry>& CachedEntries =
		SupabaseService->GetCachedLeaderboard();
	if (!CachedEntries.IsEmpty())
	{
		PopulateLeaderboard(CachedEntries);
	}

	if (!SupabaseService->IsSubmissionInProgress()
		&& !SupabaseService->IsLeaderboardRequestInProgress())
	{
		SupabaseService->GetLeaderboard();
	}
}

void UScoreboardScreenWidget::NativeDestruct()
{
	if (SupabaseService)
	{
		SupabaseService->OnLeaderboardUpdated.RemoveAll(this);
	}

	SupabaseService = nullptr;
	Super::NativeDestruct();
}

void UScoreboardScreenWidget::PopulateCurrentSession(
	const UDiplomaGameGameInstance* GameInstance
)
{
	if (!GameInstance || !SizeBox_CurrentSessionRow
		|| !ScoreboardRowWidgetClass)
	{
		return;
	}

	FLeaderboardEntry CurrentSessionEntry;
	if (!GameInstance->GetLastCompletedSessionResult(
		CurrentSessionEntry.PlayerName,
		CurrentSessionEntry.Hero,
		CurrentSessionEntry.CompletionTimeMs,
		CurrentSessionEntry.EnemiesKilled,
		CurrentSessionEntry.DamageTaken
	))
	{
		return;
	}

	UScoreboardRowWidget* RowWidget =
		CreateWidget<UScoreboardRowWidget>(
			GetOwningPlayer(),
			ScoreboardRowWidgetClass
		);
	if (!RowWidget)
	{
		return;
	}

	RowWidget->SetLeaderboardEntry(CurrentSessionEntry);
	SizeBox_CurrentSessionRow->ClearChildren();
	SizeBox_CurrentSessionRow->SetHeightOverride(CurrentSessionRowHeight);
	SizeBox_CurrentSessionRow->AddChild(RowWidget);
}

void UScoreboardScreenWidget::PopulateLeaderboard(
	const TArray<FLeaderboardEntry>& Entries
)
{
	if (!VerticalBox_LeaderboardRows || !ScoreboardRowWidgetClass)
	{
		return;
	}

	VerticalBox_LeaderboardRows->ClearChildren();
	const int32 EntryCount = FMath::Min(Entries.Num(), 10);
	for (int32 Index = 0; Index < EntryCount; ++Index)
	{
		UScoreboardRowWidget* RowWidget =
			CreateWidget<UScoreboardRowWidget>(
				GetOwningPlayer(),
				ScoreboardRowWidgetClass
			);
		if (!RowWidget)
		{
			continue;
		}

		RowWidget->SetLeaderboardEntry(Entries[Index]);

		USizeBox* RowContainer = WidgetTree
			? WidgetTree->ConstructWidget<USizeBox>()
			: NewObject<USizeBox>(this);
		RowContainer->SetHeightOverride(LeaderboardRowHeight);
		RowContainer->AddChild(RowWidget);

		UVerticalBoxSlot* RowSlot =
			VerticalBox_LeaderboardRows->AddChildToVerticalBox(RowContainer);
		if (RowSlot)
		{
			RowSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
			RowSlot->SetHorizontalAlignment(HAlign_Fill);
			RowSlot->SetVerticalAlignment(VAlign_Center);
		}
	}
}
