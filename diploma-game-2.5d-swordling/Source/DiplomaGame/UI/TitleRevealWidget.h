#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleRevealWidget.generated.h"

class APlayerCharacterBase;
class UImage;
class UMaterialInstanceDynamic;
class UMaterialInterface;

UCLASS(Abstract, Blueprintable)
class DIPLOMAGAME_API UTitleRevealWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void StartReveal(APlayerCharacterBase* InPlayerCharacter);
	void StartClosingReveal(APlayerCharacterBase* InPlayerCharacter);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(
		const FGeometry& MyGeometry,
		float InDeltaTime
	) override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Reveal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Title Reveal")
	TObjectPtr<UMaterialInterface> RevealMaterial;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Title Reveal",
		meta = (ClampMin = "0.1", Units = "s")
	)
	float RevealDuration = 1.75f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Title Reveal",
		meta = (ClampMin = "0.0", Units = "s")
	)
	float RevealHoldDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Title Reveal")
	float StartRadius = 0.07f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Title Reveal")
	float EndRadius = 2.1f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Title Reveal",
		meta = (ClampMin = "-1.0", ClampMax = "1.0")
	)
	float AutoWalkInput = -0.3f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Title Reveal",
		meta = (ClampMin = "0.0", Units = "s")
	)
	float AutoWalkDuration = 0.9f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Title Reveal")
	float FocusHeight = 55.0f;

private:
	void UpdateReveal(float NormalizedTime);
	void CompleteReveal();

	UPROPERTY(Transient)
	TObjectPtr<APlayerCharacterBase> PlayerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> RevealMaterialInstance;

	double RevealStartTime = 0.0;
	bool bAutoWalkFinished = false;
	bool bIsClosingReveal = false;
	bool bIsRevealing = false;
};
