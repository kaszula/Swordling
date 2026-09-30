#include "Components/StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetStamina();
}

void UStaminaComponent::TickComponent(
	const float DeltaTime,
	const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RegenerationDelayRemaining > 0.0f)
	{
		RegenerationDelayRemaining = FMath::Max(
			RegenerationDelayRemaining - DeltaTime,
			0.0f
		);
		return;
	}

	if (StaminaRegenerationRate <= 0.0f
		|| CurrentStamina >= MaxStamina)
	{
		SetComponentTickEnabled(false);
		return;
	}

	const float PreviousStamina = CurrentStamina;
	CurrentStamina = FMath::Min(
		CurrentStamina + StaminaRegenerationRate * DeltaTime,
		MaxStamina
	);

	if (!FMath::IsNearlyEqual(CurrentStamina, PreviousStamina))
	{
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	}

	if (CurrentStamina >= MaxStamina)
	{
		SetComponentTickEnabled(false);
	}
}

bool UStaminaComponent::TryConsumeStamina(const float StaminaCost)
{
	if (!CanConsumeStamina(StaminaCost))
	{
		return false;
	}

	const float ClampedCost = FMath::Max(StaminaCost, 0.0f);
	if (ClampedCost <= 0.0f)
	{
		return true;
	}

	CurrentStamina = FMath::Clamp(
		CurrentStamina - ClampedCost,
		0.0f,
		MaxStamina
	);
	RegenerationDelayRemaining = FMath::Max(
		StaminaRegenerationDelay,
		0.0f
	);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	SetComponentTickEnabled(true);

	return true;
}

void UStaminaComponent::ResetStamina()
{
	MaxStamina = FMath::Max(MaxStamina, 0.0f);
	CurrentStamina = MaxStamina;
	RegenerationDelayRemaining = 0.0f;
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	SetComponentTickEnabled(false);
}

void UStaminaComponent::SetStaminaValues(
	const float NewMaxStamina,
	const float NewCurrentStamina
)
{
	MaxStamina = FMath::Max(NewMaxStamina, 0.0f);
	CurrentStamina = FMath::Clamp(
		NewCurrentStamina,
		0.0f,
		MaxStamina
	);
	RegenerationDelayRemaining = 0.0f;
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	SetComponentTickEnabled(
		StaminaRegenerationRate > 0.0f
		&& CurrentStamina < MaxStamina
	);
}

bool UStaminaComponent::CanConsumeStamina(const float StaminaCost) const
{
	return StaminaCost <= 0.0f
		|| CurrentStamina + UE_KINDA_SMALL_NUMBER >= StaminaCost;
}

float UStaminaComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

float UStaminaComponent::GetMaxStamina() const
{
	return MaxStamina;
}

float UStaminaComponent::GetStaminaPercent() const
{
	return MaxStamina > 0.0f
		? FMath::Clamp(CurrentStamina / MaxStamina, 0.0f, 1.0f)
		: 0.0f;
}
