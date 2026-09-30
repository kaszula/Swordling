#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Services/SupabaseService.h"
#include "ScoreboardRowWidget.generated.h"

class UTextBlock;

UCLASS(Abstract, Blueprintable)
class DIPLOMAGAME_API UScoreboardRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetLeaderboardEntry(const FLeaderboardEntry& Entry);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Hero;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Time;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_EnemiesKilled;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_DamageTaken;
};
