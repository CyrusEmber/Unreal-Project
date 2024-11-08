// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Binggy/Interaction/InteractionOption.h"
#include "BinggyAbilitySystemComponent.generated.h"

struct FAbilitySpecPair
{
	FGameplayAbilitySpec* AbilityTagSpec = nullptr;
	FGameplayAbilitySpec* InputTagSpec = nullptr;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, const FGameplayTag& /*InputTag*/)
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
	
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	// If the return value is nullptr, then the slot is empty
	FGameplayAbilitySpec* GetSpecsFromInputTag(const FGameplayTag& InputTag);

	// TODO: put in inside experience component?
	// Update ability status when player level up, bind as callback to level attribute change, whenever a status tag is added to a spec, the old status tag should be removed
	void UpdateAbilityStatus(const FOnAttributeChangeData& Data);

	// TODO: performance issue?
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	// The AbilityTag could be 0, then we are swapping between an equipped skill and an empty skill slot
	FAbilityStatusChanged AbilityStatusChanged;

public:
	UFUNCTION(BlueprintCallable)
	void SetCurrentInteractionOptions(TArray<FInteractionOption> Options) { CurrentInteractionOptions = Options; }

	UFUNCTION(BlueprintCallable)
	TArray<FInteractionOption> GetCurrentInteractionOptions() { return CurrentInteractionOptions; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentHitResult(TArray<FInteractionOption> Options) { CurrentInteractionOptions = Options; }

	UFUNCTION(BlueprintCallable)
	TArray<FInteractionOption> GetCurrentHitResult() { return CurrentInteractionOptions; }


protected:
	void AffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);
	
	// Clear the current ability slot for equipped ability
	void ClearSlot(const FGameplayTag& InputTag);

	// Update by GA_Interact
	UPROPERTY(BlueprintReadWrite)
	TArray<FInteractionOption> CurrentInteractionOptions;

	// Update by GA_Interact
	UPROPERTY(BlueprintReadWrite)
	FHitResult CurrentHitResult;
	
private:
	// Map from AbilityTag to AbilitySpec pointer
	TMap<FGameplayTag, FGameplayAbilitySpec*> AbilityTagToSpecMap;
};
