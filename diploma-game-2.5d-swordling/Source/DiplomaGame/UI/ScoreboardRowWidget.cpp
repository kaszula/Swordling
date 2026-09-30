#include "UI/ScoreboardRowWidget.h"

#include "Components/TextBlock.h"

void UScoreboardRowWidget::SetLeaderboardEntry(
	const FLeaderboardEntry& Entry
)
{
	const int64 TotalSeconds = FMath::Max(Entry.CompletionTimeMs, 0LL) / 1000;
	const int64 Minutes = TotalSeconds / 60;
	const int64 Seconds = TotalSeconds % 60;

	Text_Name->SetText(FText::FromString(Entry.PlayerName));
	Text_Hero->SetText(FText::FromString(Entry.Hero));
	Text_Time->SetText(FText::FromString(FString::Printf(
		TEXT("%02lld:%02lld"),
		Minutes,
		Seconds
	)));
	Text_EnemiesKilled->SetText(FText::AsNumber(Entry.EnemiesKilled));
	Text_DamageTaken->SetText(FText::AsNumber(Entry.DamageTaken));
}
