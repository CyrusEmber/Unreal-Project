#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "BinggyAbilityTypes.generated.h"

class AProjectile;
class UAbilitySystemComponent;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	FDamageEffectParams(){}
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;
	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;
	UPROPERTY()
	float BaseDamage = 0.f;
	UPROPERTY()
	float AbilityLevel = 1.f;
	UPROPERTY()
	TMap<FGameplayTag, FScalableFloat> DamageTypes = TMap<FGameplayTag, FScalableFloat>();
	UPROPERTY()
	float DebuffChance = 0.f;
	UPROPERTY()
	float DebuffDamage = 0.f;
	UPROPERTY()
	float DebuffDuration = 0.f;
	UPROPERTY()
	float DebuffFrequency = 0.f;
	UPROPERTY()
	float ImpulseMagnitude = 0.f;
	UPROPERTY()
	FVector ImpulseDirection = FVector::Zero();
	// When the projectile hit something, there is hit result
	UPROPERTY()
	FHitResult HitResult;
};
