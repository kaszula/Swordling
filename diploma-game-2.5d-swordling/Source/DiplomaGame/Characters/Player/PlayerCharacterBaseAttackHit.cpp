#include "Characters/Player/PlayerCharacterBase.h"

#include "Characters/Enemy/EnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "DiplomaGame.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void APlayerCharacterBase::PlayWeaponClashSound()
{
	UWorld* World = GetWorld();
	if (!WeaponClashSound || !World)
	{
		return;
	}

	const float CurrentTime = World->GetTimeSeconds();
	constexpr float DuplicateSoundPreventionTime = 0.1f;
	if (LastWeaponClashSoundTime >= 0.0f
		&& CurrentTime - LastWeaponClashSoundTime
			< DuplicateSoundPreventionTime)
	{
		return;
	}

	LastWeaponClashSoundTime = CurrentTime;
	UGameplayStatics::PlaySoundAtLocation(
		this,
		WeaponClashSound,
		GetActorLocation()
	);
}

void APlayerCharacterBase::PerformAttackHit()
{
	UWorld* World = GetWorld();
	if (bIsDead || bIsRespawning || bIsStunned || !World || !GetMesh())
	{
		return;
	}

	FVector AttackDirection = GetMesh()->GetRightVector();
	AttackDirection.Z = 0.0f;

	if (!AttackDirection.Normalize())
	{
		return;
	}

	const float CapsuleRadius =
		GetCapsuleComponent()->GetScaledCapsuleRadius();

	const FVector AttackCenter =
		GetActorLocation()
		+ AttackDirection * (CapsuleRadius + AttackRange * 0.5f);

	const FVector AttackHalfExtent(
		AttackRange * 0.5f,
		AttackHalfWidth,
		AttackHalfHeight
	);

	const FQuat AttackRotation =
		FRotationMatrix::MakeFromX(AttackDirection).ToQuat();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	TArray<FOverlapResult> Overlaps;

	const bool bFoundActors = World->OverlapMultiByObjectType(
		Overlaps,
		AttackCenter,
		AttackRotation,
		ObjectQueryParams,
		FCollisionShape::MakeBox(AttackHalfExtent),
		QueryParams
	);

	if (bDrawAttackDebug)
	{
		DrawDebugBox(
			World,
			AttackCenter,
			AttackHalfExtent,
			AttackRotation,
			bFoundActors ? FColor::Red : FColor::Green,
			false,
			1.0f,
			0,
			2.0f
		);
	}

	if (!bFoundActors)
	{
		return;
	}

	const float Damage = AttackDamage * CurrentAttackDamageMultiplier;
	TSet<AActor*> DamagedActors;

	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.GetActor();

		if (!IsValid(HitActor)
			|| HitActor == this
			|| DamagedActors.Contains(HitActor))
		{
			continue;
		}

		DamagedActors.Add(HitActor);

		if (UHealthComponent* TargetHealthComponent =
			HitActor->FindComponentByClass<UHealthComponent>())
		{
			AEnemyCharacterBase* Enemy =
				Cast<AEnemyCharacterBase>(HitActor);
			if (Enemy
				&& IsAttackingActor(Enemy)
				&& Enemy->IsAttackingActor(this))
			{
				PlayWeaponClashSound();
				Enemy->TryApplyStun(1.0f);
				TryApplyStun(1.0f);

				UE_LOG(
					LogDiplomaGameCombat,
					Display,
					TEXT("Weapon clash between %s and %s"),
					*GetName(),
					*Enemy->GetName()
				);
				return;
			}

			if (Enemy
				&& !bCurrentAttackStartedInAir
				&& Enemy->TryBlockPlayerGroundAttack())
			{
				UE_LOG(
					LogDiplomaGameCombat,
					Display,
					TEXT("%s blocked ground attack from %s"),
					*Enemy->GetName(),
					*GetName()
				);
				continue;
			}

			TargetHealthComponent->ApplyDamage(Damage);

			if (!TargetHealthComponent->IsDead() && Enemy)
			{
				Enemy->TryApplyStun(CurrentAttackStunChance);
			}

			UE_LOG(
				LogDiplomaGameCombat,
				Display,
				TEXT("Melee attack hit %s for %.1f damage"),
				*HitActor->GetName(),
				Damage
			);
			continue;
		}

		UGameplayStatics::ApplyDamage(
			HitActor,
			Damage,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}
}
