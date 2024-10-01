// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BinggyAbilitySystemComponent.generated.h" 

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/)
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

	// Add dynamic input tag and ability status tag for start up abilities
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	// Activate once and without input tag
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	// Update ability status when player level up, bind as callback to level attribute change, whenever a status tag is added to a spec, the old status tag should be removed
	void UpdateAbilityStatus(const FOnAttributeChangeData& Data);

	// TODO: performance issue?
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	FAbilityStatusChanged AbilityStatusChanged;


protected:
	void AffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);
private:
	// Map from AbilityTag to AbilitySpec pointer
	TMap<FGameplayTag, FGameplayAbilitySpec*> AbilityTagToSpecMap;
};
