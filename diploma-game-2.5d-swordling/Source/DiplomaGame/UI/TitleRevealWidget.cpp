#include "UI/TitleRevealWidget.h"

#include "Characters/Player/PlayerCharacterBase.h"
#include "Components/Image.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

void UTitleRevealWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Image_Reveal && RevealMaterial)
	{
		RevealMaterialInstance = UMaterialInstanceDynamic::Create(
			RevealMaterial,
			this
		);
		Image_Reveal->SetBrushFromMaterial(RevealMaterialInstance);
	}
}

void UTitleRevealWidget::StartReveal(
	APlayerCharacterBase* InPlayerCharacter
)
{
	PlayerCharacter = InPlayerCharacter;
	RevealStartTime = FPlatformTime::Seconds();
	bAutoWalkFinished = false;
	bIsClosingReveal = false;
	bIsRevealing = IsValid(PlayerCharacter)
		&& IsValid(RevealMaterialInstance);

	if (!bIsRevealing)
	{
		CompleteReveal();
		return;
	}

	UpdateReveal(0.0f);
}

void UTitleRevealWidget::StartClosingReveal(
	APlayerCharacterBase* InPlayerCharacter
)
{
	PlayerCharacter = InPlayerCharacter;
	RevealStartTime = FPlatformTime::Seconds();
	bAutoWalkFinished = false;
	bIsClosingReveal = true;
	bIsRevealing = IsValid(PlayerCharacter)
		&& IsValid(RevealMaterialInstance);

	if (!bIsRevealing)
	{
		CompleteReveal();
		return;
	}

	UpdateReveal(1.0f);
}

void UTitleRevealWidget::NativeTick(
	const FGeometry& MyGeometry,
	const float InDeltaTime
)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsRevealing || !IsValid(PlayerCharacter))
	{
		return;
	}

	const double ElapsedTime = FPlatformTime::Seconds() - RevealStartTime;
	if (ElapsedTime < AutoWalkDuration)
	{
		PlayerCharacter->ApplyTitleRevealMovement(
			AutoWalkInput * 0.75f
		);
	}
	else if (!bAutoWalkFinished)
	{
		PlayerCharacter->StopTitleRevealMovement();
		bAutoWalkFinished = true;
	}

	const float NormalizedTime = FMath::Clamp(
		(static_cast<float>(ElapsedTime) - RevealHoldDuration)
			/ FMath::Max(RevealDuration, UE_SMALL_NUMBER),
		0.0f,
		1.0f
	);
	const float RevealTime = bIsClosingReveal
		? 1.0f - NormalizedTime
		: NormalizedTime;
	UpdateReveal(RevealTime);

	if (NormalizedTime >= 1.0f)
	{
		CompleteReveal();
	}
}

void UTitleRevealWidget::NativeDestruct()
{
	if (bIsRevealing)
	{
		bIsRevealing = false;
		if (IsValid(PlayerCharacter))
		{
			if (bIsClosingReveal)
			{
				PlayerCharacter->FinishGameCompletion();
			}
			else
			{
				PlayerCharacter->FinishTitleReveal();
			}
		}
		PlayerCharacter = nullptr;
	}

	Super::NativeDestruct();
}

void UTitleRevealWidget::UpdateReveal(const float NormalizedTime)
{
	if (!RevealMaterialInstance || !PlayerCharacter)
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

	int32 ViewportWidth = 0;
	int32 ViewportHeight = 0;
	PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);
	if (ViewportWidth <= 0 || ViewportHeight <= 0)
	{
		return;
	}

	FVector2D ScreenPosition;
	const FVector FocusLocation = PlayerCharacter->GetActorLocation()
		+ FVector(0.0f, 0.0f, FocusHeight);
	if (!PlayerController->ProjectWorldLocationToScreen(
		FocusLocation,
		ScreenPosition,
		true
	))
	{
		ScreenPosition = FVector2D(
			ViewportWidth * 0.5f,
			ViewportHeight * 0.5f
		);
	}

	const FVector2D Center(
		ScreenPosition.X / ViewportWidth,
		ScreenPosition.Y / ViewportHeight
	);
	const float SmoothedTime = FMath::Pow(NormalizedTime, 2.2f);
	const float Radius = FMath::Lerp(
		StartRadius,
		EndRadius,
		SmoothedTime
	);

	RevealMaterialInstance->SetVectorParameterValue(
		TEXT("Center"),
		FLinearColor(Center.X, Center.Y, 0.0f, 0.0f)
	);
	RevealMaterialInstance->SetScalarParameterValue(
		TEXT("AspectRatio"),
		static_cast<float>(ViewportWidth) / ViewportHeight
	);
	RevealMaterialInstance->SetScalarParameterValue(
		TEXT("Radius"),
		Radius
	);
}

void UTitleRevealWidget::CompleteReveal()
{
	bIsRevealing = false;

	if (IsValid(PlayerCharacter))
	{
		if (bIsClosingReveal)
		{
			PlayerCharacter->FinishGameCompletion();
		}
		else
		{
			PlayerCharacter->FinishTitleReveal();
		}
	}

	PlayerCharacter = nullptr;
	RemoveFromParent();
}
