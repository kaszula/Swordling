#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Services/SupabaseService.h"
#include "ScoreboardScreenWidget.generated.h"

class UScoreboardRowWidget;
class USupabaseService;
class USizeBox;
class UVerticalBox;
class UDiplomaGameGameInstance;

UCLASS(Abstract, Blueprintable)
class DIPLOMAGAME_API UScoreboardScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_LeaderboardRows;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> SizeBox_CurrentSessionRow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leaderboard")
	TSubclassOf<UScoreboardRowWidget> ScoreboardRowWidgetClass;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Leaderboard",
		meta = (ClampMin = "1.0")
	)
	float LeaderboardRowHeight = 35.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Leaderboard",
		meta = (ClampMin = "1.0")
	)
	float CurrentSessionRowHeight = 30.0f;

private:
	void PopulateCurrentSession(
		const UDiplomaGameGameInstance* GameInstance
	);
	void PopulateLeaderboard(const TArray<FLeaderboardEntry>& Entries);

	UPROPERTY(Transient)
	TObjectPtr<USupabaseService> SupabaseService;
};
