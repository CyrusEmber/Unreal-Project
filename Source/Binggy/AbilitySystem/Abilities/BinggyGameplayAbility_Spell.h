// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binggy/AbilitySystem/Abilities/BinggyDamageGameplayAbility.h"
#include "BinggyGameplayAbility_Spell.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyGameplayAbility_Spell : public UBinggyDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	// Handle is like a pointer
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Binggy|Defaults")
	TSubclassOf<AProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category="Binggy|Projectile")
	void SpawnProjectile(FVector TargetLocation);

	// Combat 
	// Automatic fire

	FTimerHandle FireTimer;

	bool bCanFire = true;

	void FireSpell(FVector TargetLocation, const FGameplayEffectSpecHandle& SpecHandle);
	void StartFireTimer();
	void FireTimerFinished();
	
	// Refactor into weapon specific variable, such as character->EquippedWeapon
	float FireDelay = 0.1f;
};
