#include "Components/HealthComponent.h"

#include "DiplomaGame.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetHealth();
}

void UHealthComponent::ApplyDamage(const float DamageAmount)
{
	if (DamageAmount <= 0.0f || IsDead() || bIsInvulnerable)
	{
		return;
	}

	const float PreviousHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(
		CurrentHealth - DamageAmount,
		0.0f,
		MaxHealth
	);
	const float AppliedDamage = PreviousHealth - CurrentHealth;

	UE_LOG(
		LogDiplomaGameCombat,
		Display,
		TEXT("%s received %.1f damage. Health: %.1f / %.1f"),
		*GetNameSafe(GetOwner()),
		DamageAmount,
		CurrentHealth,
		MaxHealth
	);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	OnDamageApplied.Broadcast(AppliedDamage);
	OnDamaged.Broadcast();

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

void UHealthComponent::ResetHealth()
{
	CurrentHealth = FMath::Max(MaxHealth, 0.0f);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::SetHealthValues(
	const float NewMaxHealth,
	const float NewCurrentHealth
)
{
	MaxHealth = FMath::Max(NewMaxHealth, 0.0f);
	CurrentHealth = FMath::Clamp(
		NewCurrentHealth,
		0.0f,
		MaxHealth
	);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::SetInvulnerable(const bool bNewInvulnerable)
{
	bIsInvulnerable = bNewInvulnerable;
}

bool UHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.0f;
}

bool UHealthComponent::IsInvulnerable() const
{
	return bIsInvulnerable;
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetHealthPercent() const
{
	return MaxHealth > 0.0f
		? FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f)
		: 0.0f;
}
