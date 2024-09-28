// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BinggyAbilitySystemComponent.generated.h" 

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
// DECLARE_MULTICAST_DELEGATE(FAbilityGiven);
class UAbilityInfo;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Bind delegate
	void AbilityActorInfoSet();



	// Broadcast to message system
	FEffectAssetTags EffectAssetTags;

	// With input tag
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	// Activate once and without input tag
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	// TODO: performance issue?
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:
	void AffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle);
	
};
