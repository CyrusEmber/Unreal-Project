// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binggy/AbilitySystem/Abilities/BinggyGameplayAbility.h"
#include "BinggyGameplayAbility_Spell.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyGameplayAbility_Spell : public UBinggyGameplayAbility
{
	GENERATED_BODY()

protected:
	// Handle is like a pointer
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defaults")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defaults")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
