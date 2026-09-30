#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnStaminaChangedSignature,
	float,
	CurrentStamina,
	float,
	MaxStamina
);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DIPLOMAGAME_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool TryConsumeStamina(float StaminaCost);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void ResetStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetStaminaValues(float NewMaxStamina, float NewCurrentStamina);

	UFUNCTION(BlueprintPure, Category = "Stamina")
	bool CanConsumeStamina(float StaminaCost) const;

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetCurrentStamina() const;

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStaminaPercent() const;

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaChangedSignature OnStaminaChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina = 0.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Stamina|Regeneration",
		meta = (ClampMin = "0.0")
	)
	float StaminaRegenerationRate = 30.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Stamina|Regeneration",
		meta = (ClampMin = "0.0", Units = "s")
	)
	float StaminaRegenerationDelay = 0.6f;

private:
	float RegenerationDelayRemaining = 0.0f;
};
